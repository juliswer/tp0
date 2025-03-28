#include "client.h"
#include "commons/log.h"
#include "commons/config.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log *logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	if(logger == NULL) {
		abort();
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if(config == NULL) {
		abort();
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	char* CLAVE = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, CLAVE);

	/* ---------------- LEER DE CONSOLA ---------------- */

	printf("Guardemos logs en un archivo\n");
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(CLAVE, conexion);

	// Armamos y enviamos el paquete
	printf("Mandale mensajes al server\n");
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "test", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	bool continuar = true;

	while(continuar) {
		leido = readline("> ");
		
		if(leido == NULL || strcmp(leido, "") == 0) {
			continuar = false;
			printf("Saliendo de lectura de usuario\n");
		} else {
			log_info(logger, leido);
		}

		free(leido);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	bool continuar = true;

	// Leemos y esta vez agregamos las lineas al paquete
	while(continuar) {
		leido = readline("> ");
		
		if(leido == NULL || strcmp(leido, "") == 0) {
			continuar = false;
			printf("Saliendo de lectura de usuario\n");
		} else {
			// agregar al paquete
			agregar_a_paquete(paquete, leido, strlen(leido) + 1);
			// log_info(logger, leido);
		}

		// free(paquete);
		free(leido);
	}


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete, conexion);
	free(leido);
	free(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
