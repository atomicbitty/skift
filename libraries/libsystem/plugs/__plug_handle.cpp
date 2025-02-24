
#include <abi/Syscalls.h>

#include <libsystem/core/Plugs.h>

void __plug_handle_open(Handle *handle, const char *path, OpenFlag flags)
{
    handle->result = (Result)__syscall(SYS_HANDLE_OPEN, (int)&handle->id, (int)path, (int)flags, 0, 0);
}

void __plug_handle_close(Handle *handle)
{
    if (handle->id >= 0)
    {
        handle->result = (Result)__syscall(SYS_HANDLE_CLOSE, handle->id, 0, 0, 0, 0);
    }
}

Result __plug_handle_select(HandleSet *handles, int *selected, SelectEvent *selected_events, Timeout timeout)
{
    return (Result)__syscall(SYS_HANDLE_SELECT, (int)handles, (int)selected, (int)selected_events, timeout, 0);
}

size_t __plug_handle_read(Handle *handle, void *buffer, size_t size)
{
    size_t read;

    handle->result = (Result)__syscall(SYS_HANDLE_READ, handle->id, (int)buffer, size, (int)&read, 0);

    return read;
}

size_t __plug_handle_write(Handle *handle, const void *buffer, size_t size)
{
    size_t written;

    handle->result = (Result)__syscall(SYS_HANDLE_WRITE, handle->id, (int)buffer, size, (int)&written, 0);

    return written;
}

Result __plug_handle_call(Handle *handle, IOCall request, void *args)
{

    handle->result = (Result)__syscall(SYS_HANDLE_CALL, handle->id, request, (int)args, 0, 0);

    return handle->result;
}

int __plug_handle_seek(Handle *handle, int offset, Whence whence)
{
    handle->result = (Result)__syscall(SYS_HANDLE_SEEK, handle->id, offset, whence, 0, 0);

    return 0;
}

int __plug_handle_tell(Handle *handle, Whence whence)
{
    int offset;

    handle->result = (Result)__syscall(SYS_HANDLE_TELL, handle->id, whence, (int)&offset, 0, 0);

    return offset;
}

int __plug_handle_stat(Handle *handle, FileState *stat)
{
    handle->result = (Result)__syscall(SYS_HANDLE_STAT, handle->id, (int)stat, 0, 0, 0);

    return 0;
}

void __plug_handle_connect(Handle *handle, const char *path)
{
    handle->result = (Result)__syscall(SYS_HANDLE_CONNECT, (int)&handle->id, (int)path, 0, 0, 0);
}

void __plug_handle_accept(Handle *handle, Handle *connection_handle)
{
    handle->result = (Result)__syscall(SYS_HANDLE_ACCEPT, handle->id, (int)&connection_handle->id, 0, 0, 0);
}

Result __plug_create_pipe(int *reader_handle, int *writer_handle)
{
    return (Result)__syscall(SYS_CREATE_PIPE, (int)reader_handle, (int)writer_handle, 0, 0, 0);
}

Result __plug_create_term(int *master_handle, int *slave_handle)
{
    return (Result)__syscall(SYS_CREATE_TERM, (int)master_handle, (int)slave_handle, 0, 0, 0);
}
