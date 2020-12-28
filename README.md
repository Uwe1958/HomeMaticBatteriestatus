# HomeMaticBatteriestatus (Qt, C++)

<p>Qt Project to check status of battery-powered HomeMatic devices<br><br>
Program checks the battery status of HomeMatic devices in the HomeMatic smart home network. All battery powered devices have to be included in a separate group in the CCU. The name of this group has to be given to the program in the configuration file (see description down below). In case the battery status is low an email is sent to a dedicated mail address defined via the configuration file.<br><br>
The program can be scheduled via a cron job (a run once per day should be sufficient).<br><br>
A german translation file is included in the package.<br><br>
The program uses the DB-Access add-on for the CCU which can be downloaded from https://www.christian-luetgens.de/homematic/db-access/DB-Access.htm.
Mail creation is done using the code from <a href="https://github.com/bluetiger9/SmtpClient-for-Qt">https://github.com/bluetiger9/SmtpClient-for-Qt.<a>
</p>
<h2>Program installation</h2>
<ul>
  <li>Download source code</li>
  <li>Edit configuration file</li>
  <li>Compile with Qt (using QtCreator)</li>
  <li>Schedule it via crontab</li>
</ul>
<h2>Configuration file</h2>
<p>The configuration file holds all the variables used in the program. It has to be edited and stored in the soruce code directory.
<br><br>
<table><tbody>
  <tr><td>CCU_IP=</td><td>IP of the HomeMatic CCU in the network</td></tr>
  <tr><td>CCU_Group=</td><td>Name of the group which holds all battery-powered devices in the smart home installation</td></tr>
  <tr><td>SMTP_Host=</td><td>SMTP hostname</td></tr>
  <tr><td>SMTP_Port=465</td><td>Port for the SMTP protocol (default 465)</td></tr>  
  <tr><td>SMTP_User=</td><td>Username in the SMTP host</td></tr>
  <tr><td>SMTP_Pwd=</td><td>Password</td></tr>
  <tr><td>SMTP_From=</td><td>Sender's email address</td></tr>
  <tr><td>SMTP_To=</td><td>Recipients's email address</td></tr>
  </tbody></table>
</p>
<h2>License</h2>
<p>This project is licensed under the GNU LGPL, version 2.1.</p>
<p><strong>Copyright (c) 2020 - Uwe Nagel</strong></p>
