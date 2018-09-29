To use the programs, simply use the make command to compile all 4 programs.

Sending Emails:

Start by running ./sendServer followed by the desired port number.
ex: ./sendServer 8080
Then run ./sendClient with the IP address of the Server and the required port number.
ex: ./sendClient 146.163.112.151 8080

The following conventions apply:
use standard SMTP protocol commands while communicating with the server.
emails must be enclosed within <> braces
ex: <me@email.com>
when the body of the email is finished, return two (2) blank lines.
enter QUIT to exit the server at any time.

Recieving Emails

Start by running ./recvServer followed by the desired port number.
ex: ./recvServer 8080
Then run ./recvClient with the IP address of the Server and the required port number.
ex: ./recvClient 146.163.112.151 8080

The following conventions apply:
use HTTP protocol described in the project by entering the commands one line at a time
ex:
GET /db/haddok/ HTTP 1.1
HOST: 146.163.112.151
COUNT: 1

folders to recieve emails from must start and end with a '/'
