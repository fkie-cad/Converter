#include <windows.h>
#include <winternl.h>

#include <stdio.h>
#include <stdlib.h>



INT __cdecl main(int argc, char** argv)
{
    NTSTATUS status;
    ULONG doserror;
    
    if ( argc < 2 )
    {
        printf("Usage: NtStatusToDosError <NtStatusCode>\n");
        return -1;
    }
    
    status = strtoul(argv[1], NULL, 0);
    
    doserror = RtlNtStatusToDosError(status);
    
    printf("nt status: 0x%x\n", status);
    printf("dos error: 0x%x\n", doserror);
    
    
    return 0;
}
