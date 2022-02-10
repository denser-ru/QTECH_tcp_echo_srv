#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main()

{

	int sock;                                                                      // дескриптор сокета

	struct sockaddr_in *in_addr;                                         // структура интернет-адреса (поля)

	struct ifreq ifdata;                                                        // структура - параметр

	char      *ifname = "wlp1s0";                                              // имя интерфейса


	sock = socket(AF_INET, SOCK_DGRAM, 0);                 // открываем дескриптор сокета

	if (sock < 0) {

		printf("Не удалось открыть сокет, ошибка: %s\n", strerror(errno));

		return 1;

	}

	memset(&ifdata, 0, sizeof(ifdata));                                // очищаем структуру

	strncpy(ifdata.ifr_name, ifname, IFNAMSIZ);                       // задаем имя интерфейса

//получаем айпи адрес с помощью SIOCGIFADDR, одновременно проверяя результат

	if (ioctl(sock, SIOCGIFADDR, &ifdata) < 0) {

		printf("Не получить IP адрес для %s, ошибка: %s\n", ifname, strerror(errno));

		close(sock);

		return 1;

	}

	in_addr = (struct sockaddr_in *) &ifdata.ifr_addr;           // преобразовываем из массива байт

// в структуру sockaddr_in

	printf("Интерфейс %s IP адрес: %s\n", ifname, inet_ntoa(in_addr->sin_addr));



	struct if_nameindex*     ifNameIndex;                // структура интерфейсов и их индексов

//	sock = socket(AF_INET, SOCK_DGRAM, 0);     // открываем дескриптор сокета

	if (sock < 0) {

		printf("Не удалось открыть сокет, ошибка: %s\n", strerror(errno));

		return 1;

	}


	ifNameIndex = if_nameindex();

	if (ifNameIndex) {                                             // если удалось получить данные

		while (ifNameIndex->if_index) {             // пока имеются данные

			memset(&ifdata, 0, sizeof(ifdata));                                           // очищаем структуру

			strncpy(ifdata.ifr_name, ifNameIndex->if_name, IFNAMSIZ);      // получаем имя следующего интерфейса

// получаем IP адрес с помощью SIOCGIFADDR, одновременно проверяя результат

			if (ioctl(sock, SIOCGIFADDR, &ifdata) < 0) {

				printf("Не получить IP адрес для %s, ошибка: %s\n", ifdata.ifr_name, strerror(errno));

				close(sock);

				return 1;

			}

// преобразовываем из массива байт в структуру sockaddr_in

			in_addr = (struct sockaddr_in *) &ifdata.ifr_addr;

			printf("Интерфейс %s индекес %i IP адрес: %s\n", ifdata.ifr_name, ifNameIndex->if_index, inet_ntoa(in_addr->sin_addr));

			ioctl(sock, SIOCGIFFLAGS, &ifdata);

			if (ifdata.ifr_flags & IFF_UP) {

				printf("Сетевой интерфейс %s запущен\n", ifdata.ifr_name);

			}
			else {

				ifdata.ifr_flags |= IFF_UP;
				ioctl(sock, SIOCSIFFLAGS, &ifdata);

				printf("Сетевой интерфейс был %s не запущен, попытка запустить\n", ifdata.ifr_name);

			}

			++ifNameIndex;                                   // переходим к следующему интерфейсу

		}

	}



	close(sock);

	return 0;

}