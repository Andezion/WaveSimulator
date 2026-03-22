# WaveSimulator

Визуализатор графиков на кастомных параметрах, а так же сохранение и математические операции

## Наш проект
* [Общая информация](#general-info)
* [Демонстрация](#demonstration)
* [Технологии](#technologies)
* [Важно](#features)
* [Установка](#setup)


## <a id="general-info">Общая информация</a>

Моя мини программа показывающая то, как будут выглядеть графики сигналов в зависимости от их параметров. Так же попробовал написать базовую математическую логику сложения, вычетания, умножения и деления. Используем готовые формулы сигналов - переписанные в плюсах.

## <a id="demonstration">Демонстрация</a>

1) Вот тут мы только запустили нашу программу

https://github.com/user-attachments/assets/bb87e45c-26e2-42ce-b377-6dbefca307ba

2) Вот тут мы выбрали графики и сгенерировали на дефолтных параметрах

https://github.com/user-attachments/assets/a6503161-3556-4a6e-a3e8-331fef054979

3) Вот так вписываем наши параметры

https://github.com/user-attachments/assets/0be1c4d4-e08d-4a1b-b065-646b3373a3d7

4) Сохранение и отчитывание графика

// Скоро будет

5) Операции на графиках

// Скоро будет

## <a id="technologies">Технологии</a>

Написано на С++ и Raylib. Формулы и математика написаны самостоятельно без использования дополнительных библиотек. Для билдинга выбрал Makefile.

## <a id="features">Важно</a>

Опишу нашу структуру, что где лежит и что конкретно делает

```
WavesAndSignals/
├── main.cpp                                    — точка входа: инициализация окна Raylib, главный цикл (ввод -> clamp scroll -> отрисовка)
├── app_state.h / app_state.cpp                 — глобальное состояние приложения: выбранный тип сигнала, параметры,
│                                                 текстовые поля, текущий/результирующий сигналы, статистика, флаги диалогов
├── CMakeLists.txt                              — сборка проекта: подключает Raylib (или скачивает через FetchContent),
│                                                 собирает все .cpp в один бинарник WaveSimulator
│
├── waves_math/                                 — математика сигналов (каждый сигнал — отдельный .h/.cpp)
│   ├── signal.h / signal.cpp                   — базовый класс Signal и GenericSignal;
│   │                                             generate() — наполняет samples/times по формуле;
│   │                                             mean(), absMean(), rms(), variance(), power() — статистика
│   ├── sinus_signal.*                          — S3: синусоидальный сигнал
│   ├── square_wave_signal.*                    — S4: прямоугольный сигнал (несимметричный, с dutyCycle)
│   ├── symmetrical_square_wave_signal.*        — S5: симметричный прямоугольный сигнал
│   ├── triangular_wave_signal.*                — S6: треугольный сигнал
│   ├── single_half_rectified_sinus_signal.*    — S7: однополупериодный выпрямленный синус
│   ├── double_half_rectified_sinus_signal.*    — S8: двухполупериодный выпрямленный синус
│   ├── unit_step.*                             — S9: единичный скачок (функция Хевисайда)
│   ├── unit_impulse.*                          — S10: единичный импульс (дельта-функция, дискретный)
│   ├── gaussian_noise.*                        — S1: гауссовский (нормальный) шум
│   ├── impulse_noise.*                         — S2: импульсный шум
│   └── uniform_distributed_noise.*             — S11: равномерно распределённый шум
│
├── window_handlers/                            — всё про отрисовку интерфейса (Raylib)
│   ├── draw_window.h / draw_window.cpp         — главная функция drawWindow(): собирает всё окно
│   │                                             (левая панель + график + гистограмма + диалоги)
│   ├── left_panel.h / left_panel.cpp           — левая панель: кнопки выбора сигнала, поля ввода параметров,
│   │                                             блок статистики, кнопки Generate/Save/Load и операций (+/-/×/÷)
│   ├── signal_plot.h / signal_plot.cpp         — отрисовка графика сигнала с осями, сеткой, скроллом и зумом
│   ├── histogram_plot.h / histogram_plot.cpp   — отрисовка гистограммы распределения значений сигнала
│   ├── file_dialog.h / file_dialog.cpp         — диалоговое окно ввода пути к файлу (сохранить / загрузить)
│   ├── ui_widgets.h / ui_widgets.cpp           — переиспользуемые виджеты: drawButton(), drawTextInput(),
│   │                                             handleTextInputKey()
│   └── ui_font.h / ui_font.cpp                 — глобальные цвета, константы размеров (PANEL_W, FONT_SZ),
│                                                 drawText(), measureText()
│
├── input_handlers/                             — обработка пользовательского ввода
│   └── user_input.cpp                          — processInput(): колёсико мыши (скролл / Ctrl+колёсико = зум),
│                                                 Escape (закрыть диалог / снять фокус), применение операции над сигналами
│
├── camera_handlers/                            — управление "камерой" (видовой областью) графика
│   └── sliding_window.cpp                      — clampPlotScroll(): не даёт scrollX выйти за пределы
│                                                 допустимого диапазона при текущем зуме
│
└── file_handlers/                              — чтение и запись сигналов на диск
    ├── file_io.h                               — интерфейс: saveSignal() и loadSignal()
    ├── file_output.cpp                         — сериализация сигнала (samples + times) в файл
    └── file_input.cpp                          — десериализация файла обратно в GenericSignal
```

## <a id="setup">Установка</a>


