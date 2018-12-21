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
- System Version:      Windows 10 Enterprise 1803 - 10.0.17134 (Desktop)
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
- Token Group:     NT AUTHORITY\\LogonSessionId_0_878320
- Token Group:     \\LOCAL
- Token Group:     \\Authentication authority asserted identity
---------------------------------------------------------------

- PATH:

- C:\WINDOWS\system32
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
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - R X
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - R X

- C:\WINDOWS
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
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - R X
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - R X

- C:\WINDOWS\System32\Wbem
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
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - R X
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - R X

- C:\WINDOWS\System32\WindowsPowerShell\v1.0\
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
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL APPLICATION PACKAGES - R X
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - F
APPLICATION PACKAGE AUTHORITY\ALL RESTRICTED APP PACKAGES - R X

- C:\WINDOWS\System32\OpenSSH\
------------------------------
TESTDOMAIN\w.ten - F
NT AUTHORITY\SYSTEM - F
NT AUTHORITY\LogonSessionId_0_878320 - F

- C:\Users\w.ten\AppData\Local\Microsoft\WindowsApps
----------------------------------------------------
NT AUTHORITY\SYSTEM - F
BUILTIN\Administrators - F
TESTDOMAIN\w.ten - F


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
- 88       - Unknown                   - Registry
- 308      - Unknown                   - smss.exe
- 424      - Unknown                   - csrss.exe
- 496      - Unknown                   - wininit.exe
- 504      - Unknown                   - csrss.exe
- 592      - Unknown                   - winlogon.exe
- 600      - Unknown                   - services.exe
- 640      - Unknown                   - lsass.exe
- 732      - Unknown                   - fontdrvhost.exe
- 740      - Unknown                   - fontdrvhost.exe
- 760      - Unknown                   - svchost.exe
- 840      - Unknown                   - svchost.exe
- 888      - Unknown                   - svchost.exe
- 940      - Unknown                   - svchost.exe
- 284      - Unknown                   - dwm.exe
- 488      - Unknown                   - svchost.exe
- 568      - Unknown                   - svchost.exe
- 812      - Unknown                   - svchost.exe
- 856      - Unknown                   - svchost.exe
- 1052     - Unknown                   - svchost.exe
- 1092     - Unknown                   - svchost.exe
- 1140     - Unknown                   - svchost.exe
- 1224     - Unknown                   - svchost.exe
- 1232     - Unknown                   - svchost.exe
- 1300     - Unknown                   - svchost.exe
- 1348     - Unknown                   - svchost.exe
- 1436     - Unknown                   - svchost.exe
- 1468     - Unknown                   - svchost.exe
- 1552     - Unknown                   - svchost.exe
- 1564     - Unknown                   - svchost.exe
- 1584     - Unknown                   - vmacthlp.exe
- 1692     - Unknown                   - svchost.exe
- 1716     - Unknown                   - svchost.exe
- 1724     - Unknown                   - svchost.exe
- 1852     - Unknown                   - Memory Compression
- 1888     - Unknown                   - svchost.exe
- 1940     - Unknown                   - svchost.exe
- 1948     - Unknown                   - svchost.exe
- 2024     - Unknown                   - svchost.exe
- 1792     - Unknown                   - svchost.exe
- 2084     - Unknown                   - svchost.exe
- 2112     - Unknown                   - svchost.exe
- 2128     - Unknown                   - svchost.exe
- 2148     - Unknown                   - svchost.exe
- 2160     - Unknown                   - svchost.exe
- 2284     - Unknown                   - svchost.exe
- 2292     - Unknown                   - svchost.exe
- 2428     - Unknown                   - spoolsv.exe
- 2532     - Unknown                   - svchost.exe
- 2728     - Unknown                   - svchost.exe
- 2736     - Unknown                   - svchost.exe
- 2744     - Unknown                   - svchost.exe
- 2752     - Unknown                   - svchost.exe
- 2788     - Unknown                   - svchost.exe
- 2796     - Unknown                   - VGAuthService.exe
- 2812     - Unknown                   - svchost.exe
- 2820     - Unknown                   - vmtoolsd.exe
- 2828     - Unknown                   - SecurityHealthService.exe
- 2852     - Unknown                   - MsMpEng.exe
- 2908     - Unknown                   - svchost.exe
- 3036     - Unknown                   - svchost.exe
- 3532     - Unknown                   - svchost.exe
- 3608     - Unknown                   - dllhost.exe
- 3696     - Unknown                   - WmiPrvSE.exe
- 3884     - Unknown                   - msdtc.exe
- 3224     - Unknown                   - NisSrv.exe
- 2596     - Unknown                   - svchost.exe
- 4328     - Unknown                   - svchost.exe
- 4368     - Unknown                   - svchost.exe
- 4884     - Unknown                   - svchost.exe
- 5032     - Unknown                   - SgrmBroker.exe
- 3876     - Unknown                   - svchost.exe
- 716      - Unknown                   - SearchIndexer.exe
- 3716     - Unknown                   - svchost.exe
- 4192     - Unknown                   - svchost.exe
- 2472     - Unknown                   - svchost.exe
- 4032     - Unknown                   - svchost.exe
- 1780     - w.ten                     - sihost.exe
- 3496     - w.ten                     - svchost.exe
- 3328     - w.ten                     - svchost.exe
- 5052     - Unknown                   - svchost.exe
- 1772     - Unknown                   - svchost.exe
- 2408     - Unknown                   - ctfmon.exe
- 1484     - w.ten                     - explorer.exe
- 404      - Unknown                   - svchost.exe
- 3156     - Unknown                   - svchost.exe
- 1964     - w.ten                     - taskhostw.exe
- 2192     - Unknown                   - svchost.exe
- 3728     - Unknown                   - svchost.exe
- 2108     - w.ten                     - dllhost.exe
- 852      - w.ten                     - svchost.exe
- 5272     - w.ten                     - ShellExperienceHost.exe
- 5424     - w.ten                     - SearchUI.exe
- 5468     - w.ten                     - RuntimeBroker.exe
- 5512     - w.ten                     - RuntimeBroker.exe
- 5572     - w.ten                     - RuntimeBroker.exe
- 4776     - w.ten                     - dllhost.exe
- 1608     - w.ten                     - ApplicationFrameHost.exe
- 344      - w.ten                     - MicrosoftEdge.exe
- 6108     - w.ten                     - browser_broker.exe
- 3416     - w.ten                     - RuntimeBroker.exe
- 532      - w.ten                     - MicrosoftEdgeCP.exe
- 2768     - w.ten                     - MicrosoftEdgeCP.exe
- 3572     - w.ten                     - MSASCuiL.exe
- 3432     - w.ten                     - FileCoAuth.exe
- 208      - w.ten                     - vmtoolsd.exe
- 2692     - w.ten                     - SkypeHost.exe
- 6372     - Unknown                   - svchost.exe
- 6872     - Unknown                   - audiodg.exe
- 7160     - w.ten                     - Microsoft.Photos.exe
- 4544     - w.ten                     - RuntimeBroker.exe
- 7360     - Unknown                   - svchost.exe
- 7484     - w.ten                     - RuntimeBroker.exe
- 7868     - w.ten                     - OneDrive.exe
- 7272     - w.ten                     - backgroundTaskHost.exe
- 6216     - w.ten                     - WinStore.App.exe
- 4508     - w.ten                     - RuntimeBroker.exe
- 7576     - w.ten                     - WinSecCheck.exe
- 7616     - w.ten                     - conhost.exe

[+] Listing local user accounts:
--------------------------------
- Administrator                  - Built-in account for administering the c...
- Consultant                     - N/A...
- DefaultAccount                 - A user account managed by the system....
- defaultuser0                   - N/A...
- Guest                          - Built-in account for guest access to the...
- WDAGUtilityAccount             - A user account managed and used by the s...

[+] Listing local groups:
-------------------------
- Access Control Assistance Operators - Members of this group can remotely query authorization attri...
- Administrators                      - Administrators have complete and unrestricted access to the ...
- Backup Operators                    - Backup Operators can override security restrictions for the ...
- Cryptographic Operators             - Members are authorized to perform cryptographic operations....
- Device Owners                       - Members of this group can change system-wide settings....
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
