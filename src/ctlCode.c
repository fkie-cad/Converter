#include <windows.h>
#include <winioctl.h>

#include <stdio.h>
#include <stdlib.h>

#include "print.h"
 
    
#define ACCESS_FROM_CTL_CODE(ctrlCode)          (((ULONG)(ioctl & 0xC000)) >> 14)
#define FUNCTION_FROM_CTL_CODE(ctrlCode)        (((ULONG)(ioctl & 0x3FFC)) >> 2)

PCHAR getDeviceTypeString(_In_ ULONG DeviceType);
PCHAR getMethodString(_In_ ULONG Method);
PCHAR getAccessString(_In_ ULONG Method, _Out_ PCHAR Buffer, ULONG BufferSize);

INT __cdecl main(int argc, char** argv)
{
    if ( argc < 2 )
    {
        printf("Usage: %s <DeviceType> <Function> <Method> <Access> | <ioctl>\n", argv[0]);
        return -1;
    }
    
    if ( argc >= 5 )
    {
        ULONG deviceType = strtoul(argv[1], NULL, 0);
        ULONG function = strtoul(argv[2], NULL, 0);
        ULONG method = strtoul(argv[3], NULL, 0);
        ULONG access = strtoul(argv[4], NULL, 0);
        ULONG ioctl = CTL_CODE(deviceType, function, method, access);
        
        PRINT_INT_H(deviceType, "");
        PRINT_INT_H(function, "");
        PRINT_INT_H(method, "");
        PRINT_INT_H(access, "");
        
        printf("\n");
        PRINT_INT_H(ioctl, "");
    }
    else
    {
        ULONG ioctl = strtoul(argv[1], NULL, 0);
     
        // 31             16 15 14 13             2 1 0
        // |  DeviceType   | | Ac| | Function     | |m|
        ULONG deviceType = DEVICE_TYPE_FROM_CTL_CODE(ioctl);
        ULONG function = FUNCTION_FROM_CTL_CODE(ioctl);
        ULONG method = METHOD_FROM_CTL_CODE(ioctl);
        ULONG access =ACCESS_FROM_CTL_CODE(ioctl);
            
        printf("ioctl: 0x%x\n", ioctl);
        printf("\n");

        CHAR accessBuffer[0x40];
        ULONG accessBufferSize = 0x40;

        printf("deviceType: 0x%x (%s)\n", deviceType, getDeviceTypeString(deviceType));
        printf("function: 0x%x\n", function);
        printf("method: 0x%x (%s)\n", method, getMethodString(method));
        printf("access: 0x%x (%s)\n", access, getAccessString(access, accessBuffer, accessBufferSize));
    }
    
    return 0;
}

PCHAR getDeviceTypeString(_In_ ULONG DeviceType)
{
    switch ( DeviceType )
    {
        case FILE_DEVICE_BEEP: return "FILE_DEVICE_BEEP";
        case FILE_DEVICE_CD_ROM: return "FILE_DEVICE_CD_ROM";
        case FILE_DEVICE_CD_ROM_FILE_SYSTEM: return "FILE_DEVICE_CD_ROM_FILE_SYSTEM";
        case FILE_DEVICE_CONTROLLER: return "FILE_DEVICE_CONTROLLER";
        case FILE_DEVICE_DATALINK: return "FILE_DEVICE_DATALINK";
        case FILE_DEVICE_DFS: return "FILE_DEVICE_DFS";
        case FILE_DEVICE_DISK: return "FILE_DEVICE_DISK";
        case FILE_DEVICE_DISK_FILE_SYSTEM: return "FILE_DEVICE_DISK_FILE_SYSTEM";
        case FILE_DEVICE_FILE_SYSTEM: return "FILE_DEVICE_FILE_SYSTEM";
        case FILE_DEVICE_INPORT_PORT: return "FILE_DEVICE_INPORT_PORT";
        case FILE_DEVICE_KEYBOARD: return "FILE_DEVICE_KEYBOARD";
        case FILE_DEVICE_MAILSLOT: return "FILE_DEVICE_MAILSLOT";
        case FILE_DEVICE_MIDI_IN: return "FILE_DEVICE_MIDI_IN";
        case FILE_DEVICE_MIDI_OUT: return "FILE_DEVICE_MIDI_OUT";
        case FILE_DEVICE_MOUSE: return "FILE_DEVICE_MOUSE";
        case FILE_DEVICE_MULTI_UNC_PROVIDER: return "FILE_DEVICE_MULTI_UNC_PROVIDER";
        case FILE_DEVICE_NAMED_PIPE: return "FILE_DEVICE_NAMED_PIPE";
        case FILE_DEVICE_NETWORK: return "FILE_DEVICE_NETWORK";
        case FILE_DEVICE_NETWORK_BROWSER: return "FILE_DEVICE_NETWORK_BROWSER";
        case FILE_DEVICE_NETWORK_FILE_SYSTEM: return "FILE_DEVICE_NETWORK_FILE_SYSTEM";
        case FILE_DEVICE_NULL: return "FILE_DEVICE_NULL";
        case FILE_DEVICE_PARALLEL_PORT: return "FILE_DEVICE_PARALLEL_PORT";
        case FILE_DEVICE_PHYSICAL_NETCARD: return "FILE_DEVICE_PHYSICAL_NETCARD";
        case FILE_DEVICE_PRINTER: return "FILE_DEVICE_PRINTER";
        case FILE_DEVICE_SCANNER: return "FILE_DEVICE_SCANNER";
        case FILE_DEVICE_SERIAL_MOUSE_PORT: return "FILE_DEVICE_SERIAL_MOUSE_PORT";
        case FILE_DEVICE_SERIAL_PORT: return "FILE_DEVICE_SERIAL_PORT";
        case FILE_DEVICE_SCREEN: return "FILE_DEVICE_SCREEN";
        case FILE_DEVICE_SOUND: return "FILE_DEVICE_SOUND";
        case FILE_DEVICE_STREAMS: return "FILE_DEVICE_STREAMS";
        case FILE_DEVICE_TAPE: return "FILE_DEVICE_TAPE";
        case FILE_DEVICE_TAPE_FILE_SYSTEM: return "FILE_DEVICE_TAPE_FILE_SYSTEM";
        case FILE_DEVICE_TRANSPORT: return "FILE_DEVICE_TRANSPORT";
        case FILE_DEVICE_UNKNOWN: return "FILE_DEVICE_UNKNOWN";
        case FILE_DEVICE_VIDEO: return "FILE_DEVICE_VIDEO";
        case FILE_DEVICE_VIRTUAL_DISK: return "FILE_DEVICE_VIRTUAL_DISK";
        case FILE_DEVICE_WAVE_IN: return "FILE_DEVICE_WAVE_IN";
        case FILE_DEVICE_WAVE_OUT: return "FILE_DEVICE_WAVE_OUT";
        case FILE_DEVICE_8042_PORT: return "FILE_DEVICE_8042_PORT";
        case FILE_DEVICE_NETWORK_REDIRECTOR: return "FILE_DEVICE_NETWORK_REDIRECTOR";
        case FILE_DEVICE_BATTERY: return "FILE_DEVICE_BATTERY";
        case FILE_DEVICE_BUS_EXTENDER: return "FILE_DEVICE_BUS_EXTENDER";
        case FILE_DEVICE_MODEM: return "FILE_DEVICE_MODEM";
        case FILE_DEVICE_VDM: return "FILE_DEVICE_VDM";
        case FILE_DEVICE_MASS_STORAGE: return "FILE_DEVICE_MASS_STORAGE";
        case FILE_DEVICE_SMB: return "FILE_DEVICE_SMB";
        case FILE_DEVICE_KS: return "FILE_DEVICE_KS";
        case FILE_DEVICE_CHANGER: return "FILE_DEVICE_CHANGER";
        case FILE_DEVICE_SMARTCARD: return "FILE_DEVICE_SMARTCARD";
        case FILE_DEVICE_ACPI: return "FILE_DEVICE_ACPI";
        case FILE_DEVICE_DVD: return "FILE_DEVICE_DVD";
        case FILE_DEVICE_FULLSCREEN_VIDEO: return "FILE_DEVICE_FULLSCREEN_VIDEO";
        case FILE_DEVICE_DFS_FILE_SYSTEM: return "FILE_DEVICE_DFS_FILE_SYSTEM";
        case FILE_DEVICE_DFS_VOLUME: return "FILE_DEVICE_DFS_VOLUME";
        case FILE_DEVICE_SERENUM: return "FILE_DEVICE_SERENUM";
        case FILE_DEVICE_TERMSRV: return "FILE_DEVICE_TERMSRV";
        case FILE_DEVICE_KSEC: return "FILE_DEVICE_KSEC";
        case FILE_DEVICE_FIPS: return "FILE_DEVICE_FIPS";
        case FILE_DEVICE_INFINIBAND: return "FILE_DEVICE_INFINIBAND";
        case FILE_DEVICE_VMBUS: return "FILE_DEVICE_VMBUS";
        case FILE_DEVICE_CRYPT_PROVIDER: return "FILE_DEVICE_CRYPT_PROVIDER";
        case FILE_DEVICE_WPD: return "FILE_DEVICE_WPD";
        case FILE_DEVICE_BLUETOOTH: return "FILE_DEVICE_BLUETOOTH";
        case FILE_DEVICE_MT_COMPOSITE: return "FILE_DEVICE_MT_COMPOSITE";
        case FILE_DEVICE_MT_TRANSPORT: return "FILE_DEVICE_MT_TRANSPORT";
        case FILE_DEVICE_BIOMETRIC: return "FILE_DEVICE_BIOMETRIC";
        case FILE_DEVICE_PMI: return "FILE_DEVICE_PMI";
        case FILE_DEVICE_EHSTOR: return "FILE_DEVICE_EHSTOR";
        case FILE_DEVICE_DEVAPI: return "FILE_DEVICE_DEVAPI";
        case FILE_DEVICE_GPIO: return "FILE_DEVICE_GPIO";
        case FILE_DEVICE_USBEX: return "FILE_DEVICE_USBEX";
        case FILE_DEVICE_CONSOLE: return "FILE_DEVICE_CONSOLE";
        case FILE_DEVICE_NFP: return "FILE_DEVICE_NFP";
        case FILE_DEVICE_SYSENV: return "FILE_DEVICE_SYSENV";
        case FILE_DEVICE_VIRTUAL_BLOCK: return "FILE_DEVICE_VIRTUAL_BLOCK";
        case FILE_DEVICE_POINT_OF_SERVICE: return "FILE_DEVICE_POINT_OF_SERVICE";
        case FILE_DEVICE_STORAGE_REPLICATION: return "FILE_DEVICE_STORAGE_REPLICATION";
        case FILE_DEVICE_TRUST_ENV: return "FILE_DEVICE_TRUST_ENV";
        case FILE_DEVICE_UCM: return "FILE_DEVICE_UCM";
        case FILE_DEVICE_UCMTCPCI: return "FILE_DEVICE_UCMTCPCI";
        case FILE_DEVICE_PERSISTENT_MEMORY: return "FILE_DEVICE_PERSISTENT_MEMORY";
        case FILE_DEVICE_NVDIMM: return "FILE_DEVICE_NVDIMM";
        case FILE_DEVICE_HOLOGRAPHIC: return "FILE_DEVICE_HOLOGRAPHIC";
        case FILE_DEVICE_SDFXHCI: return "FILE_DEVICE_SDFXHCI";
        case FILE_DEVICE_UCMUCSI: return "FILE_DEVICE_UCMUCSI";
        case FILE_DEVICE_PRM: return "FILE_DEVICE_PRM";
        case FILE_DEVICE_EVENT_COLLECTOR: return "FILE_DEVICE_EVENT_COLLECTOR";
        case FILE_DEVICE_USB4: return "FILE_DEVICE_USB4";
        case FILE_DEVICE_SOUNDWIRE: return "FILE_DEVICE_SOUNDWIRE";
        default: return "Unknown";
    }
}

PCHAR getMethodString(_In_ ULONG Method)
{
    switch ( Method )
    {
        case METHOD_BUFFERED: return "METHOD_BUFFERED";
        case METHOD_IN_DIRECT: return "METHOD_IN_DIRECT";
        case METHOD_OUT_DIRECT: return "METHOD_OUT_DIRECT";
        case METHOD_NEITHER: return "METHOD_NEITHER";
        default: return "Unknown";
    }
}

PCHAR getAccessString(_In_ ULONG Method, _Out_ PCHAR Buffer, ULONG BufferSize)
{
    PCHAR ptr = Buffer;
    PCHAR sep = NULL;
    ULONG restSize = BufferSize;
    Buffer[0] = 0;
    INT cch;

    if ( Method == FILE_ANY_ACCESS )
    {
        sep = ( ptr != Buffer ) ? " | " : "";
        cch = sprintf_s(ptr, restSize, "%s%s", sep, "FILE_ANY_ACCESS");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
    }
    if ( Method & FILE_READ_DATA )
    {
        sep = ( ptr != Buffer ) ? " | " : "";
        cch = sprintf_s(ptr, restSize, "%s%s", sep, "FILE_READ_DATA");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
    }
    if ( Method & FILE_WRITE_DATA )
    {
        sep = ( ptr != Buffer ) ? " | " : "";
        cch = sprintf_s(ptr, restSize, "%s%s", sep, "FILE_WRITE_DATA");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
    }
    if ( ptr == Buffer )
    {
        cch = sprintf_s(ptr, restSize, "%s", "unknown");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
    }

    return Buffer;
}
