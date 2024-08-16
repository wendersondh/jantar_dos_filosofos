# jantar_dos_filosofos

O código resolve o problema dos filósofos usando semáforos para gerenciar o acesso aos garfos e evitar deadlocks. Cada filósofo alterna entre pensar e comer, precisando de dois garfos para comer. Utiliza um semáforo mutex para controlar o acesso à região crítica e semáforos individuais para cada filósofo. A função pegar_garfos tenta adquirir garfos e a função colocar_garfos os devolve, notificando vizinhos famintos. Threads representam filósofos, alternando entre pensar e comer conforme os garfos estão disponíveis.
