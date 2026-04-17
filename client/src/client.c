#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config(logger);

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	log_info(logger, "%s", valor);
	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexion creada exitosamente");
	
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	log_info(logger, "Enviado el mensaje: %s", valor);

	// Armamos y enviamos el paquete
	paquete(conexion, logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "Cliente", 1, LOG_LEVEL_INFO);
	log_info(nuevo_logger, "Soy un Log muy Piola");

	return nuevo_logger;
}

t_config* iniciar_config(t_log* logger)
{
	t_config* nuevo_config = config_create ("cliente.config");

	if (nuevo_config == NULL){
		log_error(logger, "No se pudo crear el archivo cliente.config");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "") != 0){
		log_info(logger, "%s", leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion, t_log* logger)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	log_info(logger, "Preparando paquete...");
	leido = readline("> ");

	// Leemos y esta vez agregamos las lineas al paquete
	while(strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		log_info(logger, "Agregado al paquete: %s", leido);
		free(leido);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);
	log_info(logger, "Paquete enviado exitosamente");
	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_info(logger, "Terminando conexion...");
	log_destroy(logger);
	config_destroy(config);
}
