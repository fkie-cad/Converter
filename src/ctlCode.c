#include <windows.h>
#include <winioctl.h>

#include <stdio.h>
#include <stdlib.h>

#include "print.h"
#include "utils/strings.h"
 
#define EXE_NAME "CtlCode"
#define EXE_VS "1.0.2"
#define EXE_LC "30.04.2024"
    
#define ACCESS_FROM_CTL_CODE(ctrlCode)          (((ULONG)((ioctl) & 0xC000)) >> 14)
#define FUNCTION_FROM_CTL_CODE(ctrlCode)        (((ULONG)((ioctl) & 0x3FFC)) >> 2)

PCHAR getDeviceTypeString(_In_ ULONG DeviceType);
PCHAR getMethodString(_In_ ULONG Method);
PCHAR getAccessString(_In_ ULONG Method, _Out_ PCHAR Buffer, ULONG BufferSize);

ULONG deviceTypeStringToInt(_In_ PCHAR DeviceType);
ULONG methodStringToInt(_In_ PCHAR Method);
ULONG accessStringToInt(_In_ PCHAR Access);



INT __cdecl main(int argc, char** argv)
{
    CHAR accessBuffer[0x40];
    ULONG accessBufferSize = 0x40;

    if ( argc < 2 )
    {
        printf("Usage: %s <DeviceType> <Function> <Method> <Access> | <ioctl>\n", argv[0]);
        return -1;
    }
    
    if ( argc >= 5 )
    {

        ULONG deviceType = 0;
        ULONG method = 0;
        ULONG access = 0;
        if ( startsWith(argv[1], "FILE_DEVICE_") )
            deviceType = deviceTypeStringToInt(argv[1]);
        else
            deviceType = strtoul(argv[1], NULL, 0);

        ULONG function = strtoul(argv[2], NULL, 0);
        
        if ( startsWith(argv[3], "METHOD_") )
            method = methodStringToInt(argv[3]);
        else
            method = strtoul(argv[3], NULL, 0);
        
        if ( startsWith(argv[4], "FILE_") )
            access = accessStringToInt(argv[4]);
        else
            access = strtoul(argv[4], NULL, 0);
        ULONG ioctl = CTL_CODE(deviceType, function, method, access);
        
        printf("deviceType: 0x%x (%s)\n", deviceType, getDeviceTypeString(deviceType));
        printf("function: 0x%x\n", function);
        printf("method: 0x%x (%s)\n", method, getMethodString(method));
        printf("access: 0x%x (%s)\n", access, getAccessString(access, accessBuffer, accessBufferSize));
        
        printf("\n");
        printf("ioctl: 0x%x\n", ioctl);
    }
    else
    {
        ULONG ioctl = strtoul(argv[1], NULL, 0);
     
        // 31             16 15 14 13             2 1 0
        // |  DeviceType   | | Ac| | Function     | |m|
        ULONG deviceType = DEVICE_TYPE_FROM_CTL_CODE(ioctl);
        ULONG function = FUNCTION_FROM_CTL_CODE(ioctl);
        ULONG method = METHOD_FROM_CTL_CODE(ioctl);
        ULONG access = ACCESS_FROM_CTL_CODE(ioctl);
            
        printf("ioctl: 0x%x\n", ioctl);
        printf("\n");

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

ULONG deviceTypeStringToInt(_In_ PCHAR DeviceType)
{
    DeviceType = strToUC(DeviceType);

    if ( strcmp(DeviceType, "FILE_DEVICE_BEEP") == 0 ) return FILE_DEVICE_BEEP;
    if ( strcmp(DeviceType, "FILE_DEVICE_CD_ROM") == 0 ) return FILE_DEVICE_CD_ROM;
    if ( strcmp(DeviceType, "FILE_DEVICE_CD_ROM_FILE_SYSTEM") == 0 ) return FILE_DEVICE_CD_ROM_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_CONTROLLER") == 0 ) return FILE_DEVICE_CONTROLLER;
    if ( strcmp(DeviceType, "FILE_DEVICE_DATALINK") == 0 ) return FILE_DEVICE_DATALINK;
    if ( strcmp(DeviceType, "FILE_DEVICE_DFS") == 0 ) return FILE_DEVICE_DFS;
    if ( strcmp(DeviceType, "FILE_DEVICE_DISK") == 0 ) return FILE_DEVICE_DISK;
    if ( strcmp(DeviceType, "FILE_DEVICE_DISK_FILE_SYSTEM") == 0 ) return FILE_DEVICE_DISK_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_FILE_SYSTEM") == 0 ) return FILE_DEVICE_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_INPORT_PORT") == 0 ) return FILE_DEVICE_INPORT_PORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_KEYBOARD") == 0 ) return FILE_DEVICE_KEYBOARD;
    if ( strcmp(DeviceType, "FILE_DEVICE_MAILSLOT") == 0 ) return FILE_DEVICE_MAILSLOT;
    if ( strcmp(DeviceType, "FILE_DEVICE_MIDI_IN") == 0 ) return FILE_DEVICE_MIDI_IN;
    if ( strcmp(DeviceType, "FILE_DEVICE_MIDI_OUT") == 0 ) return FILE_DEVICE_MIDI_OUT;
    if ( strcmp(DeviceType, "FILE_DEVICE_MOUSE") == 0 ) return FILE_DEVICE_MOUSE;
    if ( strcmp(DeviceType, "FILE_DEVICE_MULTI_UNC_PROVIDER") == 0 ) return FILE_DEVICE_MULTI_UNC_PROVIDER;
    if ( strcmp(DeviceType, "FILE_DEVICE_NAMED_PIPE") == 0 ) return FILE_DEVICE_NAMED_PIPE;
    if ( strcmp(DeviceType, "FILE_DEVICE_NETWORK") == 0 ) return FILE_DEVICE_NETWORK;
    if ( strcmp(DeviceType, "FILE_DEVICE_NETWORK_BROWSER") == 0 ) return FILE_DEVICE_NETWORK_BROWSER;
    if ( strcmp(DeviceType, "FILE_DEVICE_NETWORK_FILE_SYSTEM") == 0 ) return FILE_DEVICE_NETWORK_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_NULL") == 0 ) return FILE_DEVICE_NULL;
    if ( strcmp(DeviceType, "FILE_DEVICE_PARALLEL_PORT") == 0 ) return FILE_DEVICE_PARALLEL_PORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_PHYSICAL_NETCARD") == 0 ) return FILE_DEVICE_PHYSICAL_NETCARD;
    if ( strcmp(DeviceType, "FILE_DEVICE_PRINTER") == 0 ) return FILE_DEVICE_PRINTER;
    if ( strcmp(DeviceType, "FILE_DEVICE_SCANNER") == 0 ) return FILE_DEVICE_SCANNER;
    if ( strcmp(DeviceType, "FILE_DEVICE_SERIAL_MOUSE_PORT") == 0 ) return FILE_DEVICE_SERIAL_MOUSE_PORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_SERIAL_PORT") == 0 ) return FILE_DEVICE_SERIAL_PORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_SCREEN") == 0 ) return FILE_DEVICE_SCREEN;
    if ( strcmp(DeviceType, "FILE_DEVICE_SOUND") == 0 ) return FILE_DEVICE_SOUND;
    if ( strcmp(DeviceType, "FILE_DEVICE_STREAMS") == 0 ) return FILE_DEVICE_STREAMS;
    if ( strcmp(DeviceType, "FILE_DEVICE_TAPE") == 0 ) return FILE_DEVICE_TAPE;
    if ( strcmp(DeviceType, "FILE_DEVICE_TAPE_FILE_SYSTEM") == 0 ) return FILE_DEVICE_TAPE_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_TRANSPORT") == 0 ) return FILE_DEVICE_TRANSPORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_UNKNOWN") == 0 ) return FILE_DEVICE_UNKNOWN;
    if ( strcmp(DeviceType, "FILE_DEVICE_VIDEO") == 0 ) return FILE_DEVICE_VIDEO;
    if ( strcmp(DeviceType, "FILE_DEVICE_VIRTUAL_DISK") == 0 ) return FILE_DEVICE_VIRTUAL_DISK;
    if ( strcmp(DeviceType, "FILE_DEVICE_WAVE_IN") == 0 ) return FILE_DEVICE_WAVE_IN;
    if ( strcmp(DeviceType, "FILE_DEVICE_WAVE_OUT") == 0 ) return FILE_DEVICE_WAVE_OUT;
    if ( strcmp(DeviceType, "FILE_DEVICE_8042_PORT") == 0 ) return FILE_DEVICE_8042_PORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_NETWORK_REDIRECTOR") == 0 ) return FILE_DEVICE_NETWORK_REDIRECTOR;
    if ( strcmp(DeviceType, "FILE_DEVICE_BATTERY") == 0 ) return FILE_DEVICE_BATTERY;
    if ( strcmp(DeviceType, "FILE_DEVICE_BUS_EXTENDER") == 0 ) return FILE_DEVICE_BUS_EXTENDER;
    if ( strcmp(DeviceType, "FILE_DEVICE_MODEM") == 0 ) return FILE_DEVICE_MODEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_VDM") == 0 ) return FILE_DEVICE_VDM;
    if ( strcmp(DeviceType, "FILE_DEVICE_MASS_STORAGE") == 0 ) return FILE_DEVICE_MASS_STORAGE;
    if ( strcmp(DeviceType, "FILE_DEVICE_SMB") == 0 ) return FILE_DEVICE_SMB;
    if ( strcmp(DeviceType, "FILE_DEVICE_KS") == 0 ) return FILE_DEVICE_KS;
    if ( strcmp(DeviceType, "FILE_DEVICE_CHANGER") == 0 ) return FILE_DEVICE_CHANGER;
    if ( strcmp(DeviceType, "FILE_DEVICE_SMARTCARD") == 0 ) return FILE_DEVICE_SMARTCARD;
    if ( strcmp(DeviceType, "FILE_DEVICE_ACPI") == 0 ) return FILE_DEVICE_ACPI;
    if ( strcmp(DeviceType, "FILE_DEVICE_DVD") == 0 ) return FILE_DEVICE_DVD;
    if ( strcmp(DeviceType, "FILE_DEVICE_FULLSCREEN_VIDEO") == 0 ) return FILE_DEVICE_FULLSCREEN_VIDEO;
    if ( strcmp(DeviceType, "FILE_DEVICE_DFS_FILE_SYSTEM") == 0 ) return FILE_DEVICE_DFS_FILE_SYSTEM;
    if ( strcmp(DeviceType, "FILE_DEVICE_DFS_VOLUME") == 0 ) return FILE_DEVICE_DFS_VOLUME;
    if ( strcmp(DeviceType, "FILE_DEVICE_SERENUM") == 0 ) return FILE_DEVICE_SERENUM;
    if ( strcmp(DeviceType, "FILE_DEVICE_TERMSRV") == 0 ) return FILE_DEVICE_TERMSRV;
    if ( strcmp(DeviceType, "FILE_DEVICE_KSEC") == 0 ) return FILE_DEVICE_KSEC;
    if ( strcmp(DeviceType, "FILE_DEVICE_FIPS") == 0 ) return FILE_DEVICE_FIPS;
    if ( strcmp(DeviceType, "FILE_DEVICE_INFINIBAND") == 0 ) return FILE_DEVICE_INFINIBAND;
    if ( strcmp(DeviceType, "FILE_DEVICE_VMBUS") == 0 ) return FILE_DEVICE_VMBUS;
    if ( strcmp(DeviceType, "FILE_DEVICE_CRYPT_PROVIDER") == 0 ) return FILE_DEVICE_CRYPT_PROVIDER;
    if ( strcmp(DeviceType, "FILE_DEVICE_WPD") == 0 ) return FILE_DEVICE_WPD;
    if ( strcmp(DeviceType, "FILE_DEVICE_BLUETOOTH") == 0 ) return FILE_DEVICE_BLUETOOTH;
    if ( strcmp(DeviceType, "FILE_DEVICE_MT_COMPOSITE") == 0 ) return FILE_DEVICE_MT_COMPOSITE;
    if ( strcmp(DeviceType, "FILE_DEVICE_MT_TRANSPORT") == 0 ) return FILE_DEVICE_MT_TRANSPORT;
    if ( strcmp(DeviceType, "FILE_DEVICE_BIOMETRIC") == 0 ) return FILE_DEVICE_BIOMETRIC;
    if ( strcmp(DeviceType, "FILE_DEVICE_PMI") == 0 ) return FILE_DEVICE_PMI;
    if ( strcmp(DeviceType, "FILE_DEVICE_EHSTOR") == 0 ) return FILE_DEVICE_EHSTOR;
    if ( strcmp(DeviceType, "FILE_DEVICE_DEVAPI") == 0 ) return FILE_DEVICE_DEVAPI;
    if ( strcmp(DeviceType, "FILE_DEVICE_GPIO") == 0 ) return FILE_DEVICE_GPIO;
    if ( strcmp(DeviceType, "FILE_DEVICE_USBEX") == 0 ) return FILE_DEVICE_USBEX;
    if ( strcmp(DeviceType, "FILE_DEVICE_CONSOLE") == 0 ) return FILE_DEVICE_CONSOLE;
    if ( strcmp(DeviceType, "FILE_DEVICE_NFP") == 0 ) return FILE_DEVICE_NFP;
    if ( strcmp(DeviceType, "FILE_DEVICE_SYSENV") == 0 ) return FILE_DEVICE_SYSENV;
    if ( strcmp(DeviceType, "FILE_DEVICE_VIRTUAL_BLOCK") == 0 ) return FILE_DEVICE_VIRTUAL_BLOCK;
    if ( strcmp(DeviceType, "FILE_DEVICE_POINT_OF_SERVICE") == 0 ) return FILE_DEVICE_POINT_OF_SERVICE;
    if ( strcmp(DeviceType, "FILE_DEVICE_STORAGE_REPLICATION") == 0 ) return FILE_DEVICE_STORAGE_REPLICATION;
    if ( strcmp(DeviceType, "FILE_DEVICE_TRUST_ENV") == 0 ) return FILE_DEVICE_TRUST_ENV;
    if ( strcmp(DeviceType, "FILE_DEVICE_UCM") == 0 ) return FILE_DEVICE_UCM;
    if ( strcmp(DeviceType, "FILE_DEVICE_UCMTCPCI") == 0 ) return FILE_DEVICE_UCMTCPCI;
    if ( strcmp(DeviceType, "FILE_DEVICE_PERSISTENT_MEMORY") == 0 ) return FILE_DEVICE_PERSISTENT_MEMORY;
    if ( strcmp(DeviceType, "FILE_DEVICE_NVDIMM") == 0 ) return FILE_DEVICE_NVDIMM;
    if ( strcmp(DeviceType, "FILE_DEVICE_HOLOGRAPHIC") == 0 ) return FILE_DEVICE_HOLOGRAPHIC;
    if ( strcmp(DeviceType, "FILE_DEVICE_SDFXHCI") == 0 ) return FILE_DEVICE_SDFXHCI;
    if ( strcmp(DeviceType, "FILE_DEVICE_UCMUCSI") == 0 ) return FILE_DEVICE_UCMUCSI;
    if ( strcmp(DeviceType, "FILE_DEVICE_PRM") == 0 ) return FILE_DEVICE_PRM;
    if ( strcmp(DeviceType, "FILE_DEVICE_EVENT_COLLECTOR") == 0 ) return FILE_DEVICE_EVENT_COLLECTOR;
    if ( strcmp(DeviceType, "FILE_DEVICE_USB4") == 0 ) return FILE_DEVICE_USB4;
    if ( strcmp(DeviceType, "FILE_DEVICE_SOUNDWIRE") == 0 ) return FILE_DEVICE_SOUNDWIRE;
    
    return (ULONG)-1;
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

ULONG methodStringToInt(_In_ PCHAR Method)
{
    Method = strToUC(Method);

    if ( strcmp(Method, "METHOD_BUFFERED") == 0 ) return METHOD_BUFFERED;
    if ( strcmp(Method, "METHOD_IN_DIRECT") == 0 ) return METHOD_IN_DIRECT;
    if ( strcmp(Method, "METHOD_OUT_DIRECT") == 0 ) return METHOD_OUT_DIRECT;
    if ( strcmp(Method, "METHOD_NEITHER") == 0 ) return METHOD_NEITHER;
    
    return (ULONG)-1;
}

PCHAR getAccessString(_In_ ULONG Access, _Out_ PCHAR Buffer, ULONG BufferSize)
{
    PCHAR ptr = Buffer;
    PCHAR sep = NULL;
    ULONG restSize = BufferSize;
    Buffer[0] = 0;
    INT cch;

    if ( Access == FILE_ANY_ACCESS )
    {
        sep = ( ptr != Buffer ) ? " | " : "";
        cch = sprintf_s(ptr, restSize, "%s%s", sep, "FILE_ANY_ACCESS");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
        return Buffer;
    }

    if ( Access & FILE_READ_DATA )
    {
        sep = ( ptr != Buffer ) ? " | " : "";
        cch = sprintf_s(ptr, restSize, "%s%s", sep, "FILE_READ_DATA");
        if ( cch <= 0 ) return Buffer;
        restSize -= (ULONG)cch;
        ptr += (ULONG)cch;
    }
    if ( Access & FILE_WRITE_DATA )
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

ULONG accessStringToInt(_In_ PCHAR Access)
{
    size_t bucket_max = 5;
    char* bucket[5] = { 0 };
    size_t nr_elem = split(Access, "|", bucket, bucket_max);

    if ( !nr_elem )
        return (ULONG)-1;

    ULONG access = 0;
    BOOL valid = FALSE;

    for ( size_t i = 0; i < nr_elem; i++ )
    {
        bucket[i] = strToUC(bucket[i]);

        if ( strcmp(bucket[i], "FILE_ANY_ACCESS") == 0 )
        {
            access |= FILE_ANY_ACCESS;
            valid = TRUE;
        }
        else if ( strcmp(bucket[i], "FILE_READ_DATA") == 0 
               || strcmp(bucket[i], "FILE_READ_ACCESS") == 0 )
        {
            access |= FILE_READ_DATA;
            valid = TRUE;
        }
        else if ( strcmp(bucket[i], "FILE_WRITE_DATA") == 0 
               || strcmp(bucket[i], "FILE_WRITE_ACCESS") == 0 )
        {
            access |= FILE_WRITE_DATA;
            valid = TRUE;
        }
    }

    if ( valid )
        return access;
    else
        return (ULONG)-1;
}
