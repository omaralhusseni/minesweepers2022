import socket

print("current host ip: ", socket.gethostbyname(socket.gethostname()))

HOST = socket.gethostbyname(socket.gethostname())
PORT = 5054       # Port to listen on (non-privileged ports are > 1023)


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('Socket created')


try:
	sock.bind((HOST, PORT))
except socket.error as msg:
	print ('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + str(msg[1]))
	exit

print ('Socket bind complete')

sock.listen(1)
print ('Socket now listening')

conn, addr = sock.accept()

while 1:
    data = conn.recv(32)
    print(data.decode("utf-8"))
    conn.sendall(b"OK")
