
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>

#include "kernel/node/Handle.h"
#include "kernel/node/Terminal.h"

#define TERMINAL_RINGBUFFER_SIZE 1024

static bool terminal_can_read(FsTerminal *terminal, FsHandle *handle)
{
    __unused(handle);

    if (fshandle_has_flag(handle, OPEN_MASTER))
    {
        return !ringbuffer_is_empty(terminal->slave_to_master_buffer) || !terminal->writers;
    }
    else
    {
        return !ringbuffer_is_empty(terminal->master_to_slave_buffer) || !terminal->master;
    }
}

static bool terminal_can_write(FsTerminal *terminal, FsHandle *handle)
{
    __unused(handle);

    if (fshandle_has_flag(handle, OPEN_MASTER))
    {
        return !ringbuffer_is_full(terminal->master_to_slave_buffer) || !terminal->readers;
    }
    else
    {
        return !ringbuffer_is_full(terminal->slave_to_master_buffer) || !terminal->master;
    }
}

static Result terminal_read(FsTerminal *terminal, FsHandle *handle, void *buffer, size_t size, size_t *read)
{
    __unused(handle);

    if (fshandle_has_flag(handle, OPEN_MASTER))
    {
        if (!terminal->writers)
        {
            return ERR_STREAM_CLOSED;
        }

        *read = ringbuffer_read(terminal->slave_to_master_buffer, (char *)buffer, size);
    }
    else
    {
        if (!terminal->master)
        {
            return ERR_STREAM_CLOSED;
        }

        *read = ringbuffer_read(terminal->master_to_slave_buffer, (char *)buffer, size);
    }

    return SUCCESS;
}

static Result terminal_write(FsTerminal *terminal, FsHandle *handle, const void *buffer, size_t size, size_t *written)
{
    __unused(handle);

    if (fshandle_has_flag(handle, OPEN_MASTER))
    {
        if (!terminal->readers)
        {
            return ERR_STREAM_CLOSED;
        }

        *written = ringbuffer_write(terminal->master_to_slave_buffer, (const char *)buffer, size);
    }
    else
    {
        if (!terminal->master)
        {
            return ERR_STREAM_CLOSED;
        }

        *written = ringbuffer_write(terminal->slave_to_master_buffer, (const char *)buffer, size);
    }

    return SUCCESS;
}

static Result terminal_iocall(FsTerminal *terminal, FsHandle *handle, IOCall request, void *args)
{
    __unused(handle);

    IOCallTerminalSizeArgs *size_args = (IOCallTerminalSizeArgs *)args;

    switch (request)
    {
    case IOCALL_TERMINAL_GET_SIZE:
        size_args->width = terminal->width;
        size_args->height = terminal->height;

        return SUCCESS;

    case IOCALL_TERMINAL_SET_SIZE:
        if (size_args->width < 0 || size_args->height < 0)
        {
            return ERR_INVALID_ARGUMENT;
        }

        terminal->width = size_args->width;
        terminal->height = size_args->height;

        return SUCCESS;

    default:
        return ERR_INAPPROPRIATE_CALL_FOR_DEVICE;
    }
}

static size_t terminal_size(FsTerminal *terminal, FsHandle *handle)
{
    __unused(handle);
    __unused(terminal);

    return TERMINAL_RINGBUFFER_SIZE;
}

static void terminal_destroy(FsTerminal *terminal)
{
    ringbuffer_destroy(terminal->master_to_slave_buffer);
    ringbuffer_destroy(terminal->slave_to_master_buffer);
}

FsNode *terminal_create()
{
    FsTerminal *terminal = __create(FsTerminal);

    fsnode_init(terminal, FILE_TYPE_TERMINAL);

    terminal->can_read = (FsNodeCanReadCallback)terminal_can_read;
    terminal->can_write = (FsNodeCanWriteCallback)terminal_can_write;
    terminal->read = (FsNodeReadCallback)terminal_read;
    terminal->write = (FsNodeWriteCallback)terminal_write;
    terminal->call = (FsNodeCallCallback)terminal_iocall;
    terminal->size = (FsNodeSizeCallback)terminal_size;
    terminal->destroy = (FsNodeDestroyCallback)terminal_destroy;

    terminal->width = 80;
    terminal->width = 25;

    terminal->master_to_slave_buffer = ringbuffer_create(TERMINAL_RINGBUFFER_SIZE);
    terminal->slave_to_master_buffer = ringbuffer_create(TERMINAL_RINGBUFFER_SIZE);

    return terminal;
}
