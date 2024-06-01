**Tetris**

## Opis

Prosty projekt gry Tetris, napisany w języku C++ przy użyciu biblioteki SFML. Gra skupia się na układaniu klocków w taki sposób, aby tworzyć pełne poziome linie, które znikają, zdobywając punkty. 

## Instrukcja instalacji i konfiguracji

1. Sklonuj repozytorium do wybranej lokalizacji na dysku.
2. Otwórz projekt w środowisku Visual Studio 2022.
3. Skonfiguruj projekt DLL zgodnie z instrukcjami zawartymi w pliku README.
4. Dodaj konfigurację dla biblioteki SFML, aby umożliwić poprawne kompilowanie projektu.

## Działanie programu

1. **Rozpoczęcie gry:** Po uruchomieniu gry użytkownik znajduje się w menu głównym, gdzie może rozpocząć nową grę, wyświetlić listę najlepszych wyników lub zakończyć grę.
2. **Gra:** Po rozpoczęciu nowej gry użytkownik kontroluje spadające klocki, które mogą obracać się i przesuwać w lewo lub prawo. Celem jest ułożenie klocków w taki sposób, aby tworzyły pełne linie poziome, które zostaną usunięte, zdobywając punkty.
3. **Zakończenie gry:** Gra kończy się, gdy nowy klocek nie ma miejsca na planszy do umieszczenia. Wtedy wyświetlany jest ekran końcowy z informacją o uzyskanym wyniku i możliwością zapisania wyniku gracza w tabeli najlepszych wyników.
4. **Tabela najlepszych wyników:** Gracz może przeglądać tabelę najlepszych wyników z głównego menu, gdzie wyświetlane są nazwy graczy i ich punkty.
5. **Zapisywanie wyników:** Po zakończeniu gry gracz ma możliwość zapisania swojego wyniku, podając swoją nazwę. Wynik jest automatycznie zapisywany w pliku tekstowym z wynikami.
