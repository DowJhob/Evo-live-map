# EVO livemap 

https://www.youtube.com/watch?v=l1WlNUe02IU

Приложение для редактирования таблиц контроллеров митсубиши на лету в ОЗУ. Работает с OpenPort1.3 и OpenPort2.0.
Все описания ОЗУ таблиц хранятся в хмлках экуфлэш, в описания таблиц добавлены новые атрибуты
указывающие на расположение таблиц в оперативке контроллера.

Образец:

    <table name="High Octane Fuel Map" category="Fuel" address="37b4d" type="3D" level="4" swapxy="true" scaling="AFR" RAM_addr="FFFFA04D"> 
        <table name="Load" address="4fdc" type="X Axis" elements="18" scaling="Load" RAM_mut_number="0x00"/>
        <table name="RPM" address="4fb4" type="Y Axis" elements="15" scaling="RPM" RAM_mut_number="0x02"/>
    </table>
	 RAM_addr адрес таблы в ОЗУ, RAM_mut_number ( не совсем корректное название потом поменяю ) смещение в байтах в таблице MUT в ОЗУ
  Программа сама определяет тип интерфейса J2534 или FTDI адаптер, при наличии установленных драйверов. При нажатии кнопки старт происходит попытка соединения с контроллером по альтернативному протоколу и в случае успеха читается внутренний ID, далее приложение по маске номера ID пытаеться найти и прочесть первый найденный xml файл калибровок содержащий в имени номер ID, в случае отсутствия открывается окно поиска файла. Ну а далее будут открыты все доступные для редактирования в RAM таблицы.
  Патчер <b>БУДЕТ</b> работать аналогично, при загрузке bin или hex файла читается номер калибровки и ищется xml. По нажатию кнопки apple patches все найденные в xml патчи записываются в bin.
90550001 готова но не проверена, 807ая также требует тестов.

В каталоге xdf лежат патчи для tunerpto https://www.tunerpro.net/ в дальнейшем прикручу патчер онбоард.

структура таблиц в памяти
90550001
имя таблы                |адрес в ROM|адрес в RAM|адрес в ROM|адрес в RAM
-----------------------------|-------|----------|-------|-----------
номер калибровки             |       | 90550001 |       | 80770001
MUT                          | 37B00 | FFFFA000 | 37B00 | FFFFA000
fuel                         | 37B8F | FFFFA084 | 37B8F | FFFFA084 
fuel2                        |       |          | 37CA8 | FFFFA19D
timing                       | 37CA9 | FFFFA19D | 37DC1 |
timing2                      | 37CA9 |          | 37F35 | FFFFA311
maf scal                     | 37E17 | FFFFA311 | 370A9 | FFFFA485
maf smooth                   | 37E32 | FFFFA32C | 380C4 | FFFFA4A0
latency                      | 37E47 | FFFFA347 | 380DF | FFFFA4BB
Turbo Boost Error Correction | 37E54 | FFFFA34E | 380E6 | FFFFA4C2
Boost Desired Engine Load 1  | 37E6B | FFFFA365 | 380FD | FFFFA4D9
Boost Desired Engine Load 2  | 37E81 | FFFFA37B | 38113 | FFFFA4EF
Boost Desired Engine Load 3  | 37E97 | FFFFA391 | 38129 | FFFFA505
Max Wastegate Duty 1         | 37EAD | FFFFA3A7 | 3813F | FFFFA51B
Max Wastegate Duty 2         | 37EC3 | FFFFA3BD | 38155 | FFFFA531
Max Wastegate Duty 3         | 37ED9 | FFFFA3D3 | 3816B | FFFFA547

впрочем эти данные есть xml


eulle@yandex.ru
