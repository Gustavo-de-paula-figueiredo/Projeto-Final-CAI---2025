Este projeto consiste em um carrinho robótico controlado remotamente usando um ESP32 e comunicação IoT via MQTT com o AWS IoT Core. O sistema envia dados de sensores e recebe comandos em tempo real, tudo em formato JSON.
O carrinho faz a leitura de quatro sensores de temperatura LM35 através do ADS1115 e utiliza um sensor de proximidade VL53L0X para detectar obstáculos à frente. Essas informações são publicadas periodicamente em um tópico MQTT.
A comunicação é feita de forma segura, usando TLS na porta 8883 e certificados digitais. Além de enviar dados, o ESP32 também recebe comandos para controlar a movimentação, os faróis, setas e alertas.
O sistema possui mais de um modo de funcionamento, que define como o carrinho será controlado. Esse modo fica salvo na memória do ESP32, mesmo após desligar.
O projeto é voltado para fins educacionais, unindo conceitos de sistemas embarcados, robótica e Internet das Coisas de forma prática.
