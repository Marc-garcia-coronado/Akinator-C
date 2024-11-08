#include <stdio.h>
#include <json-c/json.h>
#include <string.h>
#include <time.h>

#define MAX_TRANSFORMACIONES 10
#define MAX_PERSONAJES 25
#define MAX_NOMBRE 100
#define MAX_RANGO 50
#define MAX_TECNICA_FAVORITA 50

int preguntadas[4] = {0};
int num_preguntadas = 0;

struct Personaje
{
    char nombre[MAX_NOMBRE];
    int es_saiyan;
    int nivel_poder;
    char transformaciones[MAX_TRANSFORMACIONES][MAX_NOMBRE];
    int numero_transformaciones;
    int universo;
    char rango[MAX_RANGO];
    char tecnica_favorita[MAX_TECNICA_FAVORITA];
};

// Función para imprimir todos los personajes con sus características
void imprimir_personajes(struct Personaje lista_personajes[], int number_personajes)
{
    printf("\nLista de personajes:\n");
    for (int i = 0; i < number_personajes; i++)
    {
        printf("\nPersonaje %d:\n", i + 1);
        printf("Nombre: %s\n", lista_personajes[i].nombre);
        printf("Es Saiyan: %s\n", lista_personajes[i].es_saiyan ? "Sí" : "No");
        printf("Nivel de poder: %d\n", lista_personajes[i].nivel_poder);
        printf("Universo: %d\n", lista_personajes[i].universo);
        printf("Rango: %s\n", lista_personajes[i].rango);
        printf("Tecnica Favorita: %s\n", lista_personajes[i].tecnica_favorita);

        // Imprimir transformaciones del personaje
        printf("Transformaciones: ");
        int tiene_transformaciones = 0;
        for (int j = 0; j < lista_personajes[i].numero_transformaciones && lista_personajes[i].numero_transformaciones > 0; j++)
        {
            if (tiene_transformaciones)
                printf(", ");
            printf("%s", lista_personajes[i].transformaciones[j]);
            tiene_transformaciones = 1;
        }
        if (!tiene_transformaciones)
            printf("Ninguna");

        printf("\n"); // Nueva línea después de cada personaje
    }
    printf("\n");
}

// Función para eliminar un personaje por índice
void eliminar_personaje(struct Personaje lista_personajes[], int *number_personajes, int indice) {
    if (indice < 0 || indice >= *number_personajes) return; // Verificación adicional
    for (int i = indice; i < *number_personajes - 1; i++) {
        lista_personajes[i] = lista_personajes[i + 1];
    }
    (*number_personajes)--;
}


// Función para filtrar el array según si es Saiyan
void filtrar_por_es_saiyan(struct Personaje lista_personajes[], int *number_personajes, int es_saiyan)
{
    for (int i = 0; i < *number_personajes; i++)
    {
        if (lista_personajes[i].es_saiyan != es_saiyan)
        {
            eliminar_personaje(lista_personajes, number_personajes, i);
            i--;
        }
    }
}

// Filtrar por nivel de poder
void filtrar_por_nivel_poder(struct Personaje lista_personajes[], int *number_personajes, int nivel_poder)
{
    for (int i = 0; i < *number_personajes; i++)
    {
        if (lista_personajes[i].nivel_poder <= nivel_poder)
        {
            eliminar_personaje(lista_personajes, number_personajes, i);
            i--;
        }
    }
}

// Filtrar por universo
void filtrar_por_universo(struct Personaje lista_personajes[], int *number_personajes, int universo)
{
    if (universo == -1)
    {
        for (int i = 0; i < *number_personajes; i++)
        {
            if (lista_personajes[i].universo == 7)
            {
                eliminar_personaje(lista_personajes, number_personajes, i);
                i--;
            }
        }
        return;
    }

    for (int i = 0; i < *number_personajes; i++)
    {
        if (lista_personajes[i].universo != universo)
        {
            eliminar_personaje(lista_personajes, number_personajes, i);
            i--;
        }
    }
}

// Filtrar por transformación específica
void filtrar_por_transformacion(struct Personaje lista_personajes[], int *number_personajes, const char *transformacion)
{
    for (int i = 0; i < *number_personajes; i++)
    {
        int tiene_transformacion = 0;
        for (int j = 0; j < MAX_TRANSFORMACIONES && lista_personajes[i].transformaciones[j][0] != '\0'; j++)
        {
            if (strcmp(lista_personajes[i].transformaciones[j], transformacion) == 0)
            {
                tiene_transformacion = 1;
                break;
            }
        }
        if (!tiene_transformacion)
        {
            eliminar_personaje(lista_personajes, number_personajes, i);
            i--;
        }
    }
}
// Filtrar por transformación específica
void filtrar_por_no_transformacion(struct Personaje lista_personajes[], int *number_personajes, const char *transformacion)
{
    for (int i = 0; i < *number_personajes; i++)
    {
        int tiene_transformacion = 0;
        for (int j = 0; j < MAX_TRANSFORMACIONES && lista_personajes[i].transformaciones[j][0] != '\0'; j++)
        {
            if (strcmp(lista_personajes[i].transformaciones[j], transformacion) == 0)
            {
                tiene_transformacion = 1;
                break;
            }
        }
        if (tiene_transformacion)
        {
            eliminar_personaje(lista_personajes, number_personajes, i);
            i--;
        }
    }
}

void adivinar_personaje(struct Personaje lista_personajes[], int *number_personajes)
{
    if (*number_personajes <= 0)
    {
        printf("No hay personajes disponibles para adivinar.\n");
        return;
    }

    int adivinado = 0;
    char respuesta[10];

    do
    {
        int num_aleatorio = rand() % (*number_personajes);
        printf("¿El personaje que estás pensando es, %s? (si/no) ", lista_personajes[num_aleatorio].nombre);
        scanf("%s", respuesta);

        if (strcmp(respuesta, "no") == 0)
        {
            eliminar_personaje(lista_personajes, number_personajes, num_aleatorio);
            if (*number_personajes == 0)
                break; // Evitar seg fault si se queda vacío
        }
        else if (strcmp(respuesta, "si") == 0)
        {
            printf("\n¡El personaje que estabas pensando era %s!\n", lista_personajes[num_aleatorio].nombre);
            adivinado = 1;
            exit(0);
        }
    } while (adivinado == 0 && *number_personajes > 0);

    if (*number_personajes == 0 && adivinado == 0)
    {
        printf("No quedan personajes para adivinar.\n");
    }
}

// Función para generar una pregunta aleatoria
void hacer_pregunta_aleatoria(struct Personaje lista_personajes[], int *number_personajes)
{
    int pregunta_tipo;
    char respuesta[10];

    do
    {
        if (num_preguntadas < 4)
        {
            pregunta_tipo = (rand() % 4) + 1; // Genera un número entre 1 y 4

            // Verifica si la pregunta ya se hizo
            if (preguntadas[pregunta_tipo - 1] == 0)
            {
                // Marcar la pregunta como realizada y aumentar el contador
                preguntadas[pregunta_tipo - 1] = 1;
                num_preguntadas++;
                break;
            }
        }
        else
        {
            adivinar_personaje(lista_personajes, number_personajes);
            break; // Salir del bucle una vez que se llama a adivinar_personaje
        }
    } while (preguntadas[pregunta_tipo - 1] == 1); // Repite si ya se hizo esta pregunta

    switch (pregunta_tipo)
    {
    case 1: // Preguntar si es Saiyan
        printf("¿El personaje es Saiyan? (si/no): ");
        scanf("%s", respuesta);
        filtrar_por_es_saiyan(lista_personajes, number_personajes, strcmp(respuesta, "si") == 0 ? 1 : 0);
        break;

    case 2: // Preguntar si el nivel de poder es mayor a un valor
    {
        int nivel_referencia = 50000;
        printf("¿El personaje tiene un nivel de poder superior a %d? (si/no): ", nivel_referencia);
        scanf("%s", respuesta);
        if (strcmp(respuesta, "si") == 0)
        {
            filtrar_por_nivel_poder(lista_personajes, number_personajes, nivel_referencia);
        }
        break;
    }

    case 3: // Preguntar si pertenece al universo 7
        printf("¿El personaje pertenece al universo 7? (si/no): ");
        scanf("%s", respuesta);
        filtrar_por_universo(lista_personajes, number_personajes, strcmp(respuesta, "si") == 0 ? 7 : -1);
        break;

    case 4: // Preguntar si tiene una transformación específica
        printf("¿El personaje tiene la transformación Super Saiyan? (si/no): ");
        scanf("%s", respuesta);
        if (strcmp(respuesta, "si") == 0)
        {
            filtrar_por_transformacion(lista_personajes, number_personajes, "Super Saiyan");
        }
        else
        {
            filtrar_por_no_transformacion(lista_personajes, number_personajes, "Super Saiyan");
        }
        break;
    }
}

int main()
{
    FILE *fp;
    char buffer[7000];
    struct json_object *parsed_json, *personajes, *personaje;
    struct json_object *nombre, *es_saiyan, *nivel_poder, *transformaciones, *transformacion, *numero_transformaciones, *universo, *rango, *tecnica_favorita;
    struct Personaje lista_personajes[MAX_PERSONAJES];
    int number_personajes;
    int number_transformaciones;
    int i, y;

    // Inicialización del archivo JSON
    fp = fopen("personajes.json", "r");
    if (fp == NULL)
    {
        printf("No se puede abrir el archivo JSON\n");
        return 1;
    }

    fread(buffer, 7000, 1, fp);
    fclose(fp);
    parsed_json = json_tokener_parse(buffer);
    personajes = parsed_json;
    number_personajes = json_object_array_length(personajes);

    for (i = 0; i < number_personajes; i++)
    {
        personaje = json_object_array_get_idx(personajes, i);
        if (personaje == NULL)
            continue;

        nombre = json_object_object_get(personaje, "nombre");
        es_saiyan = json_object_object_get(personaje, "es_saiyan");
        nivel_poder = json_object_object_get(personaje, "nivel_poder");
        transformaciones = json_object_object_get(personaje, "transformaciones");
        numero_transformaciones = json_object_object_get(personaje, "numero_transformaciones");
        universo = json_object_object_get(personaje, "universo");
        rango = json_object_object_get(personaje, "rango");
        tecnica_favorita = json_object_object_get(personaje, "tecnica_favorita");

        if (rango && tecnica_favorita && nombre && es_saiyan && nivel_poder && transformaciones && universo && numero_transformaciones)
        {
            strncpy(lista_personajes[i].nombre, json_object_get_string(nombre), MAX_NOMBRE);
            strncpy(lista_personajes[i].rango, json_object_get_string(rango), MAX_RANGO);
            strncpy(lista_personajes[i].tecnica_favorita, json_object_get_string(tecnica_favorita), MAX_TECNICA_FAVORITA);
            lista_personajes[i].es_saiyan = json_object_get_boolean(es_saiyan);
            lista_personajes[i].nivel_poder = json_object_get_int(nivel_poder);
            lista_personajes[i].universo = json_object_get_int(universo);
            lista_personajes[i].numero_transformaciones = json_object_get_boolean(numero_transformaciones);

            number_transformaciones = json_object_array_length(transformaciones);
            for (y = 0; y < number_transformaciones && y < MAX_TRANSFORMACIONES; y++)
            {
                transformacion = json_object_array_get_idx(transformaciones, y);
                if (transformacion)
                {
                    strncpy(lista_personajes[i].transformaciones[y], json_object_get_string(transformacion), MAX_NOMBRE);
                }
            }
        }
        
    }

    // Realizar preguntas hasta que quede un solo personaje o el usuario quiera detenerse
    srand(time(NULL));
    while (number_personajes > 1)
    {
        printf("\nPersonajes restantes: %d\n", number_personajes);
        imprimir_personajes(lista_personajes, number_personajes);
        hacer_pregunta_aleatoria(lista_personajes, &number_personajes);
    }

    // Imprimir el personaje final
    if (number_personajes == 1)
    {
        printf("\n!!! El personaje que estabas pensando era %s !!!\n", lista_personajes[0].nombre);
    }
    else
    {
        printf("No se encontró ningún personaje que coincida con todos los filtros.\n");
    }

    return 0;
}