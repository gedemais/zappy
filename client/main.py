import sys, time, argparse

from client import Client
from bernard import IA


def	parse_arguments(arguments):
	parser = argparse.ArgumentParser(description="Un exemple de script avec des arguments")

	# Ajoutez des arguments et des options avec des descriptions
	parser.add_argument("-a", "--host", type=str, help="hostname")
	parser.add_argument("-p", "--port", type=int, help="port")
	parser.add_argument("-n", "--team_name", type=str, help="team_name")

	args = parser.parse_args(arguments)

	# Accédez aux valeurs des arguments et des options
	host = args.host
	port = args.port
	team_name = args.team_name

	if host is None:
		host = "localhost"

	if port is None or team_name is None:
		print("usage: python3 main.py -a/--host hostname -p/--port port -n/--team_name team_name")
		return host, None, None

	return host, port, team_name

def	main():
	#localhost 8080 foo
	host, port, team_name = parse_arguments(sys.argv[1:])
	if port is None or team_name is None:
		return

	client = Client(host, port, team_name)
	#connect return world's dimension
	wx, wy = client.connect()
	#time start, time, time ellapsed
	ts, t, te = 0, 0, 0
	#server sent by the server
	server_messages = []

	if wx > 0 and wy > 0:
		bernard = IA(host, port, team_name, wx, wy)
		ts = int(time.time() * 1000)
		while client.bot.alive == True:
			t = int(time.time() * 1000) - ts
			#bernard créé une cmd a transceive au serveur
			#il attend la réponse et actualise son état
			#brain lui permet de préparer des list de cmd à transceive
			#la list est executée cmd par cmd (create, transceive, receive)
			bernard.interact(t, server_messages)
			client.transceive(bernard.brain.cmd)
			server_messages = client.receive(bernard.brain.cmd)
			te = int(time.time() * 1000) - t
	client.close()

if __name__ == "__main__":
	main()
