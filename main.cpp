#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<math.h>
struct Header
{
    uint64_t  upper_level_device_id:16;
    uint64_t info:8;
    uint64_t device_type:8;
    uint64_t lower_level_devices_count:16;
    uint64_t ID:16;
};
/*
uint64_t extract64(uint64_t data, uint64_t inicio, uint64_t final)
{
    uint64_t num_bits = final - inicio + 1; // Número de bits a extraer
    uint64_t mask = (1 << num_bits) - 1; // Máscara para extraer los bits
    return (data >> inicio) & mask; // Desplazar a la derecha y aplicar máscara
}
*/
uint64_t extract64(uint64_t data, uint32_t inicio, uint32_t final) {
    uint32_t num_bits = final - inicio + 1; // Número de bits a extraer
    uint64_t mask = (1ULL << num_bits) - 1; // Máscara para extraer los bits
    return (data >> inicio) & mask; // Desplazar a la derecha y aplicar máscara
}
struct Header extract64(uint64_t encabezadoData)
{
    Header head;
    head.upper_level_device_id=extract64(encabezadoData,0,15);
    head.info=extract64(encabezadoData,16,23);
    head.device_type = extract64(encabezadoData, 24, 31);
    head.lower_level_devices_count=extract64(encabezadoData,32,47);
    head.ID=extract64(encabezadoData,48,63);
    return head;
}

int main()
{
    FILE *f=fopen("network_structure.dat","rb");
    int cant;
    fseek(f,0,SEEK_END);
    cant=(ftell(f)/sizeof(struct Header)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant);
    fseek(f,0,SEEK_SET);
    //Cantidad de paquetes es 24
    struct Header *data=new struct Header[cant];
    int result = fread(data, sizeof(Header), cant, f);
    if (result != cant) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }

    for (int i = 0; i < cant; i++)
    {
        struct Header head = extract64(data[i].device_type);
        struct Header head_1 = extract64(data[i].ID);
        struct Header head_2 = extract64(data[i].lower_level_devices_count);
        struct Header head_3 = extract64(data[i].upper_level_device_id);
        printf("ID:%u \n",head_1.ID);
        printf("Cantidad de equipos conectados:%u\n",head_2.lower_level_devices_count);
        printf("ID Del equipo conectado:%u\n",head_3.upper_level_device_id);
       if(head.device_type==0)
        {
            //head.device_type
            printf("Registro de Equipo :CPU\n");
        }
        if(head.device_type==1)
        {
            printf("Registro de equipo:SENSOR\n");
            if (head.info==0)//Sensor de caudal
            {
                printf("De tipo:Flow");
            }
            if (head.info==1)//Sensor de temperatura
            {
                printf("De tipo:Temp");
            }
            if (head.info==2)//Sensor de Presion
            {
                printf("De tipo:Presure");
            }
            if (head.info==3)//Sensor de nivel
            {
                printf("De tipo:level");
            }
        }
        if(head.device_type==2)
        {
            printf("Registro de equipo:ACTUADOR\n");
            if (head.info==0)//Electrovalvula
            {
                printf("De tipo:Electrovalvula");
            }
            if (head.info==1)
            {
                printf("De tipo:Motor");
            }
        }
        if (head.device_type==3)
        {
            printf("Registro de equipo:CONCENTRADOR\n");
        } 
    }
}

