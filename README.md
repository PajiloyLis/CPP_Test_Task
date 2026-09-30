# Telecom Client-Server

Тестовое задание: клиент-серверное приложение на Qt C++ для телекоммуникационной системы.
Сервер — GUI-приложение с таблицами подключённых клиентов и принятых данных, клиент — консольное
приложение, эмулирующее устройство и отправляющее метрики, статусы и логи.

---

## Окружение разработки и тестирования

| Параметр            | Значение         |
|---------------------|------------------|
| ОС                  | Ubuntu 24.04 LTS |
| Компилятор          | g++ 13.3.0       |
| CMake               | 4.4.3            |
| Qt                  | 6.5.2            |
| Сборочная система   | CMake            |
| Стандарт C++        | C++20            |


---

## Структура проекта

```
.
├── build.sh                                # сборка сервера и клиента
├── run.sh                                  # сборка + запуск
├── CMakeLists.txt                          # Cmake для сборки клиента и сервера
├── commonProtocolModels/                   # общие модели протокола
│   ├── Incoming.h                          # входящие данные сервера
│   ├── Outcoming.h                         # отправляемые сервером данные
│   ├── Measurements.h                      # NetworkMetrics, DeviceStatus, LogMessage
│   ├── packetCodec/                        # Преобразование данных в модели
│   │   ├── PacketCodec.cpp
│   │   └── PacketCodec.h
│   └── lineBuffer/                         # Обработка NDJSON
│       ├── LineBuffer.cpp
│       └── LineBuffer.h
├── server/                                 # сервер
│   ├── main.cpp
│   ├── ui/                                 # MainWindow, SettingsDialog
│   │   ├── MainWindow.cpp
│   │   ├── MainWindow.h
│   │   ├── MainWindow.ui                   # Дизайн MainWindow
│   │   └── settingsDialog/                 # SettingsDialog
│   │       ├── SettingsDialog.cpp
│   │       ├── SettingsDialog.h
│   │       └── SettingsDialog.ui           # Дизайн SettingsDialog
│   ├── controller/                         # IServerController, ServerController
│   │   ├── IServerController.h
│   │   └── ServerController
│   │       ├── ServerController.cpp
│   │       └── ServerController.h
│   ├── network/                            # ServerWorker, ClientSession
│   │   ├── clientSession                   # Чтение и отправка данных
│   │   │   ├── ClientSession.cpp
│   │   │   └── ClientSession.h
│   │   └── serverWorker                    # Обрабатывает принимаемые пакеты
│   │       ├── ServerWorker.cpp
│   │       └── ServerWorker.h
│   ├── configModels/                       # ServerSettings
│   │   └── ServerSettings.h                # Модель настроек сервера
│   ├── configParsers/                      # Парсеры конфигов
│   │   ├── serverConfigParser              # Парсер конфига сервера
│   │   │   ├── ServerSettingsParser.cpp
│   │   │   └── ServerSettingsParser.h
│   │   └── thresholdsConfigParser          # Парсер конфига порогов
│   │       ├── ThresholdsParser.cpp
│   │       └── ThresholdsParser.h
│   ├── domainModels/                       # ClientInfo, Thresholds, ClientStatus
│   │   ├── ClientInfo.h
│   │   ├── ClientStatus.h
│   │   └── Thresholds.h
│   └── services/                           # SettingsService
│       ├── SettingsService.cpp
│       └── SettingsService.h
└── client/                                 # клиент
    ├── main.cpp
    ├── services/                           # Клиентский сервис
    │   └── ClientService
    │       ├── ClientService.cpp
    │       └── ClientService.h
    ├── configModels/                       
    │   └── ClientSettings.h                # Модель настроек клиента
    └── configParsers/
        └── ClientConfigParser              # Парсер конфига клиента
            ├── ClientSettingsParser.cpp
            └── ClientSettingsParser.h
```

---

## Сборка

Скрипт `build.sh` собирает клиентское и серверное приложения и помещает артефакты в `./build/`:

```bash
./build.sh                # Release (по умолчанию)
./build.sh Debug          # Debug
```

---

## Запуск

Скрипт `run.sh` собирает клиентское и серверное приложения и запускает их

```bash
./run.sh              # только сервер
./run.sh client       # только клиент
./run.sh both 3       # сервер и 3 клиента в одном терминале
./run.sh 5            # то же, что и «both 5»
```

---

## Порядок работы:

1. В окне сервера нажать **Start Server** — сервер начнёт слушать порт из `server.json`.
2. Запустить одного или несколько клиентов — они подключатся и появятся в таблице «Connected Clients».
3. Нажать **Start Clients** — сервер разошлёт всем команду `Start`, и клиенты начнут отправку пакетов.
4. Наблюдать данные в таблице Client Data и события в Event Log.
5. **Stop Clients** — пауза отправки. **Stop Server** — остановка сервера, таблицы очищаются.

---

## Конфигурация

### Сервер

Файл `server.json` рядом с исполняемым файлом:

```json
{
  "port": 12345,
  "bind_address": "0.0.0.0"
}
```

### Пороги метрик

Файл `thresholds.json` рядом с исполняемым файлом. Правится через **Client Settings…** в GUI:

```json
{
  "max_latency": 50.0,
  "max_packet_loss": 5.0,
  "min_bandwidth": 10.0,
  "max_cpu_usage": 90,
  "max_memory_usage": 90,
  "send_log_on_alert": true
}
```

### Клиент

Файл `client.json` рядом с исполняемым файлом:

```json
{
  "host": "localhost",
  "port": 12345,
  "retry_interval_ms": 5000,
  "min_send_interval_ms": 10,
  "max_send_interval_ms": 100
}
```

---

## Протокол

Обмен — NDJSON: одно JSON-сообщение на строку, разделитель `\n`.

### От клиента серверу

| Тип              | Пример                                                                              |
|------------------|-------------------------------------------------------------------------------------|
| `NetworkMetrics` | `{"type":"NetworkMetrics","bandwidth":100.5,"latency":12.3,"packet_loss":0.01}`     |
| `DeviceStatus`   | `{"type":"DeviceStatus","uptime":3600,"cpu_usage":25,"memory_usage":60}`            |
| `Log`            | `{"type":"Log","message":"Interface eth0 restarted","severity":"INFO"}`             |

### От сервера клиенту

| Тип       | Пример                                                                  |
|-----------|-------------------------------------------------------------------------|
| `Welcome` | `{"type":"Welcome","assigned_id":1,"server_name":"TelecomServer/1.0"}`  |
| `Start`   | `{"type":"Start"}`                                                      |
| `Stop`    | `{"type":"Stop"}`                                                       |
| `Alert`   | `{"type":"Alert","severity":"WARN","reason":"latency 60 ms > 50 ms"}`   |

---

## Архитектура

- **Сервер** состоит из GUI-потока (`MainWindow`, `ServerController`) и worker-потока
  (`ServerWorker`, `ClientSession`). Общение между потоками — через сигналы/слоты
  с `Qt::QueuedConnection`. Сокеты и парсинг JSON живут в worker-потоке.
- **Протокол** (`commonProtocolModels/`) общий для сервера и клиента: типы, кодек
  и `LineBuffer` для обработки NDJSON.
- **Клиент** — один `ClientService` с конечным автоматом
  `Connecting → WaitingWelcome → WaitingStart → Running`. Повторный запрос подключения, отправка и
  генерация случайных данных — через `QTimer` и `QRandomGenerator`.

---

## Обработка ошибок

- Битый JSON от клиента игнорируется, ошибка пишется в лог сервера.
- Потеря соединения клиентом → автоматический ретрай каждые 5 секунд.
- Клиент, превысивший порог метрики, помечается статусом `Warning`; клиенту
  отправляется `Alert`, но не чаще одного раза в секунду.
- При остановке сервера все сессии корректно закрываются, дескрипторы освобождаются.
