# WinSecCheck
Tool to enumerate potential security misconfigurations, allowing for privilege escalation on a windows machine.

## What is it?
WinSecCheck is a tool that can be utilised in order to retrieve as much information about the host it is ran on, in order to help elevate privileges from a user or underprivileged account to a local administrative account, and potentially further...

It has been written in order for me to understand the methods to escalate, and what better way than to program them. A lot of tools nowadays are utilisng .NET or Powershell, and whilst there are already tools out there that *probably* do this a lot better than my tool will, I have decided to try and stick to using solely the Win32API.

The reason for this is to evade detection and have full compatibility. A lot of Powershell is monitored heavily by system administrations and A/V systems now. With it being written in Pure C and using Windows API, a lot of the dependencies that the other tools will rely on (.NET, Powershell, libraries etc) are not needed. This software has been tried on a test (basic) domain infrastructure comprising of the following:
- Windows Server 2016 (Latest updates) - Domain Controller.
- Windows Vista SP2 (Latest updates) - Host.
- Windows Seven SP1 (Latest updates) - Host.
- Windows 10 (Latest updates) - Host.

### So far:
- **List System Information**:

   *System Version, Service Pack, CPU Info, Installed Applications.*
- **Adapter Information**:

   *Each internet adapter, DNS entries + Gateway information.*
- **Is it domain joined?**

   *The domain name + PCs domain name.*
- **Current user information**:

   *User token Domain + User, e.g. DOMAIN\\LogonName*
   
   *User token groups, e.g. DOMAIN\\Domain Users*
- **Environment Path**

   *Permissions (DACL) of Path*   
- **Process Privileges**:

   *SeShutdownPrivilege*
   
   *SeDebugPrivilege etc...*
- **Running Processes**:

   *PID, User, Image name*
- **Installed Applications**:

   *32bit and 64bit registry keys (dependant on version ran)*
- **Recent Files**:

   *PowerShell history, Office, Windows Recent Files*
- **Local Information**

   *Local user accounts*
   
   *Local groups*
- **Domain information**

   *Loops trusts (untested with more than one DC so far...)*
   
   *Domain DNS*
   
   *Domain DC DNS*
   
   *Domain DC IP*
   
   *Domain users*
   
   *Domain groups*

There will be more added as this project develops, such as individual insecure components, registry entries, unquoted services etc. The TODO lists some of the work that is yet to be complete.

Thanks for reading this. Contribute, criticise, use. All are welcome.

- Snoom

## Usage
```
C:\Users\wseven\Desktop>WinSecCheck.exe
usage: WinSecCheck [-A] | [-uPtpardln]

Arguments:
-h, --help:             Display(this);
-A, --all:              Display everything.
-u, --user:             User information (PC/Domain & Groups).
-P, --path:             Path variable + DACL information.
-t, --tokens:           Current user token privileges.
-p, --processes:        Current running processes (PID/User/ImageName).
-a, --apps:             Installed applications (x86/x64).
-r, --recentfiles:      Recently used files - office, start-menu, PS history.
-d, --dominfo:          Domain information (Users & Groups).
-l, --localinfo:        Local information (User & Groups).
-n, --adapters:         Network Adapter information (Gateway/IP/DNS).
```

## Example Output
```
C:\Users\wseven\Desktop>WinSecCheck.exe -A
+-----------------------+
+      WinSecCheck      +
+-----------------------+
+      - thesnoom       +
+        @GitHub        +
+-----------------------+

[+] Listing system information:
-------------------------------
- System Version:      Windows 7 Professional - 6.1.7601 (Desktop)
- Service Pack:        1.0 (Service Pack 1)
- CPU Info:            x64 - 1C(2T) - Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz
------------------------------------------------------------------------------
- Domain joined PC:    testdom.local
- PC Domain Name:      wseven.testdom.local
-------------------------------------------

[+] Listing network adapter information:
----------------------------------------
- Address:   169.254.128.107  | Bluetooth Network Connection
------------------------------------------------------------
- Address:   192.168.68.103   | Local Area Connection
-----------------------------------------------------
- DNS:       192.168.68.101
- Gateway:   192.168.68.2
-------------------------
- Address:   127.0.0.1        | Loopback Pseudo-Interface 1
-----------------------------------------------------------

[+] Listing current user information:
-------------------------------------
- Token User:      TESTDOM\\wseven
----------------------------------
- Token Group:     TESTDOM\\Domain Users
- Token Group:     \\Everyone
- Token Group:     BUILTIN\\Users
- Token Group:     NT AUTHORITY\\INTERACTIVE
- Token Group:     \\CONSOLE LOGON
- Token Group:     NT AUTHORITY\\Authenticated Users
- Token Group:     NT AUTHORITY\\This Organization
- Token Group:     NT AUTHORITY\\LogonSessionId_0_2144392
- Token Group:     \\LOCAL
- Token Group:     \\Authentication authority asserted identity
---------------------------------------------------------------

- PATH:

- C:\Windows\system32
---------------------
NT SERVICE\TrustedInstaller - F
NT SERVICE\TrustedInstaller - F
NT AUTHORITY\SYSTEM - F
NT AUTHORITY\SYSTEM - F
BUILTIN\Administrators - F
BUILTIN\Administrators - F
BUILTIN\Users - F
BUILTIN\Users - R X
\CREATOR OWNER - F

- C:\Windows
------------
NT SERVICE\TrustedInstaller - F
NT SERVICE\TrustedInstaller - F
NT AUTHORITY\SYSTEM - F
NT AUTHORITY\SYSTEM - F
BUILTIN\Administrators - F
BUILTIN\Administrators - F
BUILTIN\Users - F
BUILTIN\Users - R X
\CREATOR OWNER - F

- C:\Windows\System32\Wbem
--------------------------
NT SERVICE\TrustedInstaller - F
NT SERVICE\TrustedInstaller - F
NT AUTHORITY\SYSTEM - F
NT AUTHORITY\SYSTEM - F
BUILTIN\Administrators - F
BUILTIN\Administrators - F
BUILTIN\Users - F
BUILTIN\Users - R X
\CREATOR OWNER - F

- C:\Windows\System32\WindowsPowerShell\v1.0\
---------------------------------------------
NT SERVICE\TrustedInstaller - F
NT SERVICE\TrustedInstaller - F
NT AUTHORITY\SYSTEM - F
NT AUTHORITY\SYSTEM - F
BUILTIN\Administrators - F
BUILTIN\Administrators - F
BUILTIN\Users - F
BUILTIN\Users - R X
\CREATOR OWNER - F


[+] Listing current process privileges:
---------------------------------------
- SeShutdownPrivilege                    Disabled   Shut down the system
- SeChangeNotifyPrivilege                Enabled    Bypass traverse checking
- SeUndockPrivilege                      Disabled   Remove computer from docking station
- SeIncreaseWorkingSetPrivilege          Disabled   Increase a process working set
- SeTimeZonePrivilege                    Disabled   Change the time zone

[+] Listing running processes:
------------------------------
- 0        - Unknown                   - N/A
- 4        - Unknown                   - System
- 256      - Unknown                   - smss.exe
- 348      - Unknown                   - csrss.exe
- 388      - Unknown                   - wininit.exe
- 396      - Unknown                   - csrss.exe
- 444      - Unknown                   - services.exe
- 476      - Unknown                   - winlogon.exe
- 484      - Unknown                   - lsass.exe
- 492      - Unknown                   - lsm.exe
- 616      - Unknown                   - svchost.exe
- 676      - Unknown                   - vmacthlp.exe
- 720      - Unknown                   - svchost.exe
- 796      - Unknown                   - svchost.exe
- 860      - Unknown                   - svchost.exe
- 904      - Unknown                   - svchost.exe
- 936      - Unknown                   - svchost.exe
- 692      - Unknown                   - svchost.exe
- 1168     - Unknown                   - spoolsv.exe
- 1196     - Unknown                   - svchost.exe
- 1344     - Unknown                   - svchost.exe
- 1408     - Unknown                   - VGAuthService.exe
- 1508     - Unknown                   - vmtoolsd.exe
- 1744     - Unknown                   - svchost.exe
- 1884     - Unknown                   - WmiPrvSE.exe
- 1052     - Unknown                   - msdtc.exe
- 2160     - Unknown                   - svchost.exe
- 2428     - Unknown                   - WmiPrvSE.exe
- 2832     - Unknown                   - svchost.exe
- 2868     - Unknown                   - mscorsvw.exe
- 2932     - Unknown                   - mscorsvw.exe
- 2972     - Unknown                   - sppsvc.exe
- 3016     - Unknown                   - SearchIndexer.exe
- 2620     - Unknown                   - TrustedInstaller.exe
- 2572     - Unknown                   - VSSVC.exe
- 2380     - Unknown                   - svchost.exe
- 2052     - Unknown                   - WmiApSrv.exe
- 652      - wseven                    - taskhost.exe
- 1692     - wseven                    - dwm.exe
- 2992     - wseven                    - explorer.exe
- 2784     - wseven                    - vmtoolsd.exe
- 1000     - Unknown                   - audiodg.exe
- 2284     - wseven                    - cmd.exe
- 3004     - wseven                    - conhost.exe
- 1064     - wseven                    - wuauclt.exe
- 2876     - Unknown                   - SearchProtocolHost.exe
- 264      - Unknown                   - SearchFilterHost.exe
- 2996     - wseven                    - WinSecCheck.exe

[+] Listing installed applications:
-----------------------------------
[+] 32BIT Keys:
---------------
- Microsoft Visual C++ 2017 Redistributable (x86) - 14.12.25810
- Microsoft Visual C++ 2017 x86 Additional Runtime - 14.12.25810
- Microsoft Visual C++ 2017 x86 Minimum Runtime - 14.12.25810
- Update for Microsoft .NET Framework 4.7.2 (KB4087364)
- Microsoft Visual C++ 2017 Redistributable (x64) - 14.12.25810

[+] 64BIT Keys:
---------------
- Microsoft .NET Framework 4.7.2
- Microsoft Visual C++ 2017 x64 Additional Runtime - 14.12.25810
- VMware Tools
- Microsoft .NET Framework 4.7.2
- Microsoft Visual C++ 2017 x64 Minimum Runtime - 14.12.25810

[+] PowerShell history:
-----------------------------------
- PowerShell history not found

[+] Active office recent files:
-----------------------------------

[+] Active start-menu recent files:
-----------------------------------

[+] Listing domain information:
-------------------------------
- Domain Name DNS:          testdom.local
- Domain DC DNS             \\TESTPDC.testdom.local
- Domain DC IP:             \\192.168.68.101
--------------------------------------------
- Listing domain groups:
------------------------
- Cloneable Domain Controllers   - Members of this group that are domain co...
- DnsUpdateProxy                 - DNS clients who are permitted to perform...
- Domain Admins                  - Designated administrators of the domain...
- Domain Computers               - All workstations and servers joined to t...
- Domain Controllers             - All domain controllers in the domain...
- Domain Guests                  - All domain guests...
- Domain Users                   - All domain users...
- Enterprise Admins              - Designated administrators of the enterpr...
- Enterprise Key Admins          - Members of this group can perform admini...
- Enterprise Read-only Domain Controllers - Members of this group are Read-Only Doma...
- Group Policy Creator Owners    - Members in this group can modify group p...
- Key Admins                     - Members of this group can perform admini...
- Protected Users                - Members of this group are afforded addit...
- Read-only Domain Controllers   - Members of this group are Read-Only Doma...
- Schema Admins                  - Designated administrators of the schema...

- Listing domain accounts:
--------------------------
- Administrator
---------------
-- Group Policy Creator Owners
-- Enterprise Admins
-- Domain Users
-- Domain Admins
-- Schema Admins

- DefaultAccount
----------------
-- Domain Users

- Guest
-------
-- Domain Guests

- krbtgt
--------
-- Domain Users

- wseven
--------
-- Domain Users

- wten
------
-- Domain Users

- wvista
--------
-- Domain Users


[+] Listing local user accounts:
---------------------------------
- Administrator                  - Built-in account for administering the c...
- Guest                          - Built-in account for guest access to the...
- Winten                         - N/A...

[+] Listing local groups:
--------------------------
- Administrators                      - Administrators have complete and unrestricted access to the ...
- Backup Operators                    - Backup Operators can override security restrictions for the ...
- Cryptographic Operators             - Members are authorized to perform cryptographic operations....
- Distributed COM Users               - Members are allowed to launch, activate and use Distributed ...
- Event Log Readers                   - Members of this group can read event logs from local machine...
- Guests                              - Guests have the same access as members of the Users group by...
- IIS_IUSRS                           - Built-in group used by Internet Information Services....
- Network Configuration Operators     - Members in this group can have some administrative privilege...
- Performance Log Users               - Members of this group may schedule logging of performance co...
- Performance Monitor Users           - Members of this group can access performance counter data lo...
- Power Users                         - Power Users are included for backwards compatibility and pos...
- Remote Desktop Users                - Members in this group are granted the right to logon remotel...
- Replicator                          - Supports file replication in a domain...
- Users                               - Users are prevented from making accidental or intentional sy...
```
