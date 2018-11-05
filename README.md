# WinSecCheck
Tool to enumerate potential security misconfigurations, allowing for privilege escalation on a windows machine.

## What is it?
WinSecCheck is a tool that can be utilised in order to retrieve as much information about the host it is ran on, in order to help elevate privileges from a user or service account to a local administrative account, and potentially further...

It has been written in order for me to understand the methods to escalate, and what better way than to program them. A lot of tools nowadays are utilisng .NET or Powershell, and whilst there are already tools out there that *probably* do this a lot better than my tool will, I have decided to try and stick to using solely the Win32API.

The reason for this is to evade detection and have full compatibility. A lot of Powershell is monitored heavily by system administrations and A/V systems now. With it being written in Pure C and using Windows API, a lot of the dependencies that the other tools will rely on (.NET, Powershell, libraries etc) are not needed. This software has been tried on a test (basic) domain infrastructure comprising of the following:
- Windows Server 2016 (Latest updates) - Domain Controller.
- Windows Vista SP2 (Latest updates) - Host.
- Windows Seven SP1 (Latest updates) - Host.
- Windows 10 (Latest updates) - Host.

### So far:
- **List System Information**:

   *System Version, Service Pack, CPU Info.*
- **Adapter Information**:

   *Each internet adapter, DNS entries + Gateway information.*
- **Is it domain joined?**

   *The domain name + PCs domain name.*
- **Current user information**:

   *User token Domain + User, e.g. DOMAIN\\LogonName*
   
   *User token groups, e.g. DOMAIN\\Domain Users*
- **Environment Path**
- **Process Privileges**:

   *SeShutdownPrivilege*
   
   *SeDebugPrivilege etc...*
- **Running Processes**:

   *PID, User, Image name*
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

# Credit
- MSDN
- Stackoverflow
- Google
- MWR Incognito (Tokens! <3)

# Example output:
```
+-----------------------+
+      WinSecCheck      +
+-----------------------+
+      - thesnoom       +
+        @GitHub        +
+-----------------------+

[+] Listing system information:
-------------------------------
- System Version:      Windows 10 Pro 1607 - 10.0.14393 (Desktop)
- Service Pack:        0.0 (N/A)
- CPU Info:            x64 - 1C(2T) - Intel(R) Core(TM) i7-7700HQ CPU @ 2.80GHz
-------------------------------------------------------------------------------
- Address:   192.168.37.5     | Ethernet0
-----------------------------------------
- DNS:       192.168.37.2
- DNS:       8.8.8.8
- Gateway:   192.168.37.1
-------------------------
- Address:   127.0.0.1        | Loopback Pseudo-Interface 1
-----------------------------------------------------------
- Domain joined PC:    testdomain.local
- PC Domain Name:      WIN10.testdomain.local
---------------------------------------------

[+] Listing current user information:
-------------------------------------
- Token User:      TESTDOMAIN\\w.ten
------------------------------------
- Token Group:     TESTDOMAIN\\Domain Users
- Token Group:     \\Everyone
- Token Group:     BUILTIN\\Users
- Token Group:     NT AUTHORITY\\INTERACTIVE
- Token Group:     \\CONSOLE LOGON
- Token Group:     NT AUTHORITY\\Authenticated Users
- Token Group:     NT AUTHORITY\\This Organization
- Token Group:     NT AUTHORITY\\LogonSessionId_0_417991
- Token Group:     \\LOCAL
- Token Group:     \\Authentication authority asserted identity
---------------------------------------------------------------
- PATH:
- C:\Windows\system32
- C:\Windows
- C:\Windows\System32\Wbem
- C:\Windows\System32\WindowsPowerShell\v1.0\
- C:\Users\w.ten\AppData\Local\Microsoft\WindowsApp

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
- 284      - Unknown                   - smss.exe
- 396      - Unknown                   - csrss.exe
- 468      - Unknown                   - wininit.exe
- 480      - Unknown                   - csrss.exe
- 540      - Unknown                   - services.exe
- 548      - Unknown                   - lsass.exe
- 588      - Unknown                   - winlogon.exe
- 680      - Unknown                   - svchost.exe
- 748      - Unknown                   - svchost.exe
- 860      - Unknown                   - svchost.exe
- 892      - Unknown                   - dwm.exe
- 932      - Unknown                   - svchost.exe
- 964      - Unknown                   - svchost.exe
- 1016     - Unknown                   - svchost.exe
- 484      - Unknown                   - svchost.exe
- 536      - Unknown                   - svchost.exe
- 1140     - Unknown                   - vmacthlp.exe
- 1372     - Unknown                   - svchost.exe
- 1456     - Unknown                   - svchost.exe
- 1548     - Unknown                   - spoolsv.exe
- 1696     - Unknown                   - svchost.exe
- 1828     - Unknown                   - VGAuthService.exe
- 1836     - Unknown                   - vmtoolsd.exe
- 1856     - Unknown                   - svchost.exe
- 1932     - Unknown                   - Memory Compression
- 2104     - Unknown                   - WmiPrvSE.exe
- 2256     - Unknown                   - dllhost.exe
- 2612     - Unknown                   - msdtc.exe
- 3372     - Unknown                   - svchost.exe
- 1036     - w.ten                     - sihost.exe
- 900      - w.ten                     - svchost.exe
- 3976     - w.ten                     - taskhostw.exe
- 2592     - w.ten                     - RuntimeBroker.exe
- 5744     - w.ten                     - MSASCuiL.exe
- 5780     - w.ten                     - vmtoolsd.exe
- 4128     - w.ten                     - OneDrive.exe
- 4372     - w.ten                     - ApplicationFrameHost.exe
- 2928     - w.ten                     - dllhost.exe
- 1964     - Unknown                   - svchost.exe
- 4840     - Unknown                   - sedsvc.exe
- 360      - w.ten                     - SystemSettings.exe
- 5932     - Unknown                   - SearchIndexer.exe
- 1152     - Unknown                   - MsMpEng.exe
- 5060     - Unknown                   - NisSrv.exe
- 2960     - w.ten                     - SkypeHost.exe
- 988      - Unknown                   - MpCmdRun.exe
- 5460     - w.ten                     - explorer.exe
- 7084     - w.ten                     - ShellExperienceHost.exe
- 5288     - w.ten                     - SearchUI.exe
- 528      - Unknown                   - SearchProtocolHost.exe
- 3148     - Unknown                   - SearchFilterHost.exe
- 2728     - w.ten                     - WinSecCheck.exe
- 3992     - w.ten                     - conhost.exe

[+] Listing local user accounts:
--------------------------------
- Administrator                  - Built-in account for administering the c...
- Consultant                     - N/A...
- DefaultAccount                 - A user account managed by the system....
- defaultuser0                   - N/A...
- Guest                          - Built-in account for guest access to the...

[+] Listing local groups:
-------------------------
- Access Control Assistance Operators - Members of this group can remotely query authorization attri...
- Administrators                      - Administrators have complete and unrestricted access to the ...
- Backup Operators                    - Backup Operators can override security restrictions for the ...
- Cryptographic Operators             - Members are authorized to perform cryptographic operations....
- Distributed COM Users               - Members are allowed to launch, activate and use Distributed ...
- Event Log Readers                   - Members of this group can read event logs from local machine...
- Guests                              - Guests have the same access as members of the Users group by...
- Hyper-V Administrators              - Members of this group have complete and unrestricted access ...
- IIS_IUSRS                           - Built-in group used by Internet Information Services....
- Network Configuration Operators     - Members in this group can have some administrative privilege...
- Performance Log Users               - Members of this group may schedule logging of performance co...
- Performance Monitor Users           - Members of this group can access performance counter data lo...
- Power Users                         - Power Users are included for backwards compatibility and pos...
- Remote Desktop Users                - Members in this group are granted the right to logon remotel...
- Remote Management Users             - Members of this group can access WMI resources over manageme...
- Replicator                          - Supports file replication in a domain...
- System Managed Accounts Group       - Members of this group are managed by the system....
- Users                               - Users are prevented from making accidental or intentional sy...

[+] Listing domain information:
-------------------------------
- Domain Name DNS:          testdomain.local
- Domain DC DNS             \\TEST-PDC.testdomain.local
- Domain DC IP:             \\192.168.37.2
------------------------------------------
- Listing domain accounts:
--------------------------
- Administrator                  - Built-in account for administering the c...
- DefaultAccount                 - A user account managed by the system....
- Guest                          - Built-in account for guest access to the...
- krbtgt                         - Key Distribution Center Service Account...
- w.seven                        (Win Seven User) - Seven AD Account...
- w.ten                          (Win Ten User) - Ten AD Account...
- w.vista                        (Win Vista User) - Vista AD Account...

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
```
