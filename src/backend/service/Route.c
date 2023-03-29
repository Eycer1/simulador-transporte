#include "route.h"

void initRoute(Route *route, char *cod, char *name, char *travelTime, Parade *parade, Service *service)
{
    // Se inicializa el pipe de comunicacion con el proceso padre
    pipe(route->pip);
    strcpy(route->cod, cod);
    strcpy(route->name, name);
    route->parade = parade;
    route->service = service;
    strcpy(route->travelTime, travelTime);
}

List *createRoutes(char *serviceFile, char *loadFile)
{
    // Alojando memoria inicial para la ruta
    List *routes = (List *)malloc(sizeof(List));
    initList(routes);
    if (!routes)
    {
        perror("No hay suficiente memoria en su dispositivo");
        exit(EXIT_FAILURE);
    }

    // Lectura de los datos principales de las rutas
    FILE *sfile = fopen(serviceFile, "r");
    FILE *cfile = fopen(loadFile, "r");

    // Se lee la carga
    char sline[1024];
    char cline[1024];
    fgets(cline, 1024, cfile); // Leer la cabecera del archivo de carga

    // Almacenar la hora de llegada de las cargas
    char *token = strtok(cline, ",");
    int hours[8];
    for (int i = 0; token != NULL; i++)
    {
        // Ignorar las primeras tres columnas
        if (i >= 3)
        {
            sscanf(token, "%d", &hours[i - 3]);
        }
        token = strtok(NULL, ",");
    }

    // Creacion de rutas
    while (fgets(sline, 1024, sfile) && fgets(cline, 1024, cfile))
    {
        Route *route = (Route *)malloc(sizeof(Route));
        // Se obtiene el nombre y el codigo de la ruta
        char cod[4];
        char name[50];
        char travelTime[6];
        char *ctoken = strtok(cline, ",");
        strcpy(cod, ctoken);
        ctoken = strtok(NULL, ",");
        strcpy(name, ctoken);
        ctoken = strtok(NULL, ",");
        sscanf(ctoken, "%s", travelTime);
        ctoken = strtok(NULL, ",");

        // Se crea la Parada y las cargas que llegaran a ella
        Parade *parade = createParade(ctoken, hours);

        // Se crea el servicio y se le asocian sus autobuses
        char *stoken = strtok(sline, " ");
        stoken = strtok(NULL, " ");
        Service *service = createService(stoken);

        // Se Crea la ruta y se agrega a la lista de rutas
        initRoute(route, cod, name, travelTime, parade, service);
        ListContent *content = (ListContent *)malloc(sizeof(ListContent));
        content->route = route;
        routes = listPush(routes, content);
    }
    return routes;
}