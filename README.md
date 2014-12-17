`pman` is a linux tool used to manage processes. Some of its features include

* Sending notifications when a managed process exits
* Operate on multiple processes using process ids/process names/wildcards
* Manage processes as groups
* Update a process group without having to stop/restart the processes in the group

Operations
---

A _processSpecifier_ can either be a _processID_ or a _processName_.  A _processName_ can optionally include the group name, for example, if you had a process named "WebServer" in group "MyServices", the _processName_ could be "WebServer" or "MyServices.WebServer".

> Note: _groupName_ and _processName_ cannot start with a number so they can be distinguished from process ids.

> Note: _groupName_/_processName_ support wildcard matching using '*'.

|             |         |
|-------------|---------|
| Command     | status [_processSpecifier_...] |
| Description | Queries the status of the processes given by _processSpecifier_.  If no _processSpecifier_ is given then the status of all processes is returned. |
| Returns     | The status of the given processes. |
| Example     | > status <br/> < MyServices.WebServer 2681 Running <br/> < MyServices.JetlinkSimulator 8593 Stopped <br/> < MyServices.EngineEmulation 8594 Crashed <br/> < (empty line) |
| Note        | If any _processSpecifier_ contains a wildcard then the return data must include an empty line, otherwise, the empty line is not returned. |

|             |         |
|-------------|---------|
| Command     | start [_processSpecifier_...] |
| Command     | stop [_processSpecifier_...] |
| Command     | reset [_processSpecifier_...] |
| Description | Starts/Stops/Resets the processes given by _processSpecifier_.  If no _processSpecifier_ is given then all processes are started/stopped/reset. |
| Returns     | The status of the given processes (after the operation). |
| Example     | > start MyServices.WebServer <br/> < WebServer 8493 Started |
| Example     | > start MyServices.* <br/> < MyServices.WebServer 8493 AlreadyRunning <br/> < MyServices.JelinkSimulator 7592 Started <br/> < (empty line) |
| Note        | If any _processSpecifier_ contains a wildcard then the return data must include an empty line, otherwise, the empty line is not returned. |



|             |         |
|-------------|---------|
| Command     | create _groupName_<br/> _processDefinition_ <br/> _processDefinition_ ... <br/> (empty line) |
| Description | Creates a group of process definitions. |
| Returns     | _Success_ or Error: _errorMessage_ |

|             |         |
|-------------|---------|
| Command     | update _groupName_ [create] <br/> _processDefinition_ <br/> _processDefinition_ ... <br/> (empty line) |
| Description | Updates group of process definitions. If _create_ is given and the process group does not exist, then it will be created. |
| Returns     | The status of the given processes (after the operation). |
| Example     | > updategroup MyServices <br/> > WebServer system("apache") <br/> > (empty line) <br/> < MyServices.WebServer 8493 NoChange <br/> < (empty line) |


> TODO: create format for a _processDefinition_

> WebServer system("apache /etc/apache.conf")

> JetlinkSimulator system("mono /root/test_tools/jlvm/app/JetlinkVirtualMachine.exe")

