gcc -pthread -o client networks.c client.c
gcc -pthread -o server networks.c server.c
gcc -pthread -o backup_server networks.c backup_server.c