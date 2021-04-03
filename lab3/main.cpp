/* WiFi Example
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ThisThread.h"
#include "mbed.h"
#include "TCPSocket.h"
#include "mbed_power_mgmt.h"

// Sensors drivers present in the BSP library
#include "mbed_retarget.h"
// #include "stm32l475e_iot01_tsensor.h"
// #include "stm32l475e_iot01_hsensor.h"
// #include "stm32l475e_iot01_psensor.h"
// #include "stm32l475e_iot01_magneto.h"
// #include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#include <cstdint>
#include <cstdio>

#define WIFI_IDW0XX1    2

#if (defined(TARGET_DISCO_L475VG_IOT01A) || defined(TARGET_DISCO_F413ZH))
#include "ISM43362Interface.h"
ISM43362Interface wifi(false);

#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
SpwfSAInterface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif

const char* HOST = "192.168.50.101";
const int PORT = 65435;

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

char* get_accel() {
    char* ret = new char[30];
    int16_t pDataXYZ[3] = {0};
    printf("Start sensor init\n");
    BSP_ACCELERO_Init();
    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    printf("\nACCELERO_X = %d\n", pDataXYZ[0]);
    printf("ACCELERO_Y = %d\n", pDataXYZ[1]);
    printf("ACCELERO_Z = %d\n", pDataXYZ[2]);
    int n = sprintf(ret, "{\"x\":%d,\"y\":%d,\"z\":%d}", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
    printf("n = %d", n);
    ThisThread::sleep_for(50);
    return ret;
}

void http_demo(NetworkInterface *net)
{
    TCPSocket socket;
    nsapi_error_t response;
    int16_t pDataXYZ[3] = {0};
    char* xyz = NULL;

    printf("Sending request to PC server...\n");

    // Open a socket on the network interface, and create a TCP connection to www.arm.com
    SocketAddress a;
    net->get_ip_address(&a);
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None"); 
    printf("Sending HTTP request to PC server...\n");
    // Open a socket on the network interface, and create a TCP connection to //www.arm.com
    socket.open(net); 
    a.set_ip_address(HOST);
    a.set_port(PORT);
    response = socket.connect(a);
    printf("Socket Address is %s\n", a.get_ip_address());

    if(0 != response) {
        printf("Error connecting: %d\n", response);
        socket.close();
        return;
    }

    while(1) {
        printf("Start sensor loop\n");
        xyz = get_accel();
        nsapi_size_t size = strlen(xyz);
        response = 0;
        while(size)
        {
            response = socket.send(xyz+response, size);
            if (response < 0) {
                printf("Error sending data: %d\n", response);
                socket.close();
                return;
            } else {
                size -= response;
                // Check if entire message was sent or not
                printf("sent %d [%.*s]\n", response, strstr(xyz, "\r\n")-xyz, xyz);
            }
        }
        char rbuffer[64];
        response = socket.recv(rbuffer, sizeof rbuffer);
        if (response < 0) {
            printf("Error receiving data: %d\n", response);
        } else {
            printf("recv %d [%.*s]\n", response, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);
        }
    }

    socket.close();
}


int main()
{
    printf("\nWiFi-Socket practice\n\n");
    
    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    printf("IP: %s\n", wifi.get_ip_address());
    printf("Netmask: %s\n", wifi.get_netmask());
    printf("Gateway: %s\n", wifi.get_gateway());
    printf("RSSI: %d\n\n", wifi.get_rssi());

    http_demo(&wifi);

    wifi.disconnect();

    printf("\nDone\n");
}
