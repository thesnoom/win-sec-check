# WinSecCheck
Tool to enumerate potential security misconfigurations, allowing for privilege escalation on a windows machine.

## What is it?
WinSecCheck is a tool that can be utilised in order to retrieve as much information about the host it is ran on, in order to help elevate privileges from a user or service account to a local administrative account, and potentially further...

It has been written in order for me to understand the methods to escalate, and what better way than to program them. A lot of tools nowadays are utilisng .NET or Powershell, and whilst there are already tools out there that *probably* do this a lot better than my tool will, I have decided to try and stick to using solely the Win32API.

The reason for this is to evade detection and have full compatability. A lot of Powershell is monitored heavily by system administrations and A/V systems now. With it being written in Pure C and using Windows API, a lot of the dependencies that the other tools will rely on (.NET, Powershell, libraries etc) are not needed. This software has been tried on a test (basic) domain infrastructure comprising of the following:
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

   *Loops trusts (untested so far...)*
   
   *Domain DNS*
   
   *Domain DC DNS*
   
   *Domain DC IP*
   
   *Domain users*
   
   *Domain groups*

There will be more added as this project develops, such as individual insecure components, registry entries, unquoted services etc. The TODO lists some of the work that is yet to be complete.

Thanks for reading this. Contribute, criticise, use. All are welcome.

- Snoom
