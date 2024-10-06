# Generator Raportów do Podwyższonych Kosztów Uzyskania Przychodów (PKUP)

Wersja 1.0 jest wynikiem pracy inżynierskiej ukończonej w Uniwersytecie Pomorskim w Słupsku.

## Cel projektu

Efektem tej pracy inżynierskiej jest program, który umożliwia, w sposób zautomatyzowany, przygotowanie raportu ewidencji czasu pracy twórczej w formie PDF, który pracownik może, po podpisaniu, złożyć w dziale kadr (HR) swojej firmy.

## Funkcjonalności

### Przeszukiwanie repozytoriów

Jest to pierwszy etap w zbieraniu informacji potrzebnych do realizacji zamierzonego działania. Istotne jest w nim podanie imienia i nazwiska, które będzie użyte w raporcie, adresu e-mail, którym użytkownik identyfikuje się w systemie, a także listy repozytoriów do przeszukania oraz zakresu dat, który nas interesuje. Nie trzeba przy tym wskazywać dokładnego katalogu głównego repozytorium, a jedynie podfolder.

Aby przyspieszyć proces wpisywania danych, po wyborze pierwszego repozytorium informacje o autorze są pobierane automatycznie, jeśli jeszcze nie zostały podane.

Na tym etapie istnieje możliwość zapisu ustawień przeszukiwania, co umożliwia stworzenie powtarzalnego profilu pracownika. Profile te zapisywane są na dysku.

Mechanizm przeszukiwania działa wielowątkowo w skali gałęzi zmian, na każdym z repozytoriów z osobna. Eliminuje to problem braku odpowiedzi interfejsu, a także przyspiesza operację.

### Edycja listy prac twórczych

Trafiamy tu po skończonej operacji przeszukiwania repozytoriów. Możemy tu usunąć niepotrzebne elementy na liście, a także podać, jak dużo czasu poświęciliśmy na wprowadzenie poszczególnych zmian. Czas ten wyrażamy w godzinach. Użyteczną funkcją może okazać się kalkulator, którym możemy obliczyć procentowy udział pracy twórczej w całkowitym czasie pracy, przy czym liczba dni roboczych jest mnożona razy 8 godzin. Ponieważ zwykle mamy określony limit procentowy czasu pracy twórczej w umowie, dlatego warto sprawdzać tę wartość zanim wyślemy raport.

### Podgląd i zapis raportu

Po wprowadzeniu potrzebnych informacji o pracy pozostaje nam podać miejscowość oraz datę, które zostaną umieszczone w raporcie. Pole miejscowości zostaje zachowane w trakcie zapisywania ustawień predefiniowanych na pierwszej stronie. Data zostaje wypełniona ostatnim dniem z wybranego zakresu przeszukiwania.

Na tym etapie istnieje możliwość podglądu treści jaka zostanie zapisana w postaci dokumentu PDF. Podgląd ten odświeża się co sekundę od wprowadzenia ostatniej zmiany.

Dokument zapisywany jest w formacie PDF z jednym szablonem stron A4. Zawiera on datę wygenerowania, miejscowość, imię i nazwisko pracownika, tabelę utworów, a także sumę godzin pracy twórczej oraz miejsca na podpis pracodawcy i pracownika.

Tabela zawiera kolumny: liczba porządkowa, nazwa repozytorium, data wykonania, unikatowy identyfikator, tytuł pracy oraz liczba godzin, spędzonych na wykonanie pracy.

## Używane technologie

* **CMake** – narzędzie przygotowujące środowisko budowania przyrostowego pod dany system operacyjny i kompilator. Wybrany jako konfigurator kodu tej aplikacji z uwagi na prostotę w użyciu oraz uniwersalność.

* Język **C++** w wersji *20* – popularny obiektowy język programowania wysokiego poziomu z możliwością szczegółowej kontroli użycia pamięci. Podstawowe narzędzie  w tym projekcie.

* **vcpkg** – narzędzie instalujące biblioteki zewnętrzne i narzędzia w sposób zautomatyzowany. Używany w tym projekcie do dołączania zewnętrznych bibliotek.

* **Qt** – zestaw bibliotek wieloplatformowych dających wiele gotowych rozwiązań dla skrócenia kodu. Używany tutaj do budowania głównych struktur programu.

* **QML** – biblioteka graficzna przeznaczona do przygotowywania uniwersalnych interfejsów użytkownika, która jest dostarczana z *Qt*. Daje możliwość deklaratywnego opisu struktury interfejsu graficznego. Wybrany do stworzenia interfejsu graficznego tej aplikacji ze względu na zmniejszenie ilości kodu, przy jednoczesnym zwiększeniu pozytywnych odczuć (UX/UI) dla użytkownika.

* **Git** – najpopularniejsze narzędzie kontroli wersji. Używany jest do gromadzenia kodu tego programu, jak również jest źródłem informacji o pracy twórczej pracownika.

* **libGit** – biblioteka napisana w języku C, z której korzysta również system Git. Używana jest, aby odczytać informacje dotyczących zmian z podanych repozytoriów.

* **LaTEX** – technologia wspomagająca zautomatyzowane generowanie pliku PDF.
Dzięki niej program może stworzyć tekstowy opis struktury dokumentu, a kompilator automatycznie przygotuje docelowy raport w formacie PDF.
W tej wersji programu dołączony jest w instalacji pakiet **MikTEX**, który opis tekstowy przetwarza do postaci dokumentu.

* **Google Test** – środowisko ułatwiające tworzenie testów jednostkowych. Używany do testowania najważniejszych klas w bibliotece bazowej kodu.

## Instalacja i wymagania

Aplikacja zaprojektowana jest na komputery z systemem *MS Windows 10* lub nowszym. Wymagane jest posiadanie myszy i klawiatury lub ekranu dotykowego z wirtualną klawiaturą.

Do korzystania z repozytoriów *Git* zalecane jest zainstalowanie na komputerze pakietu Git w trybie administratora.

Program jest dostarczany w formie archiwum ZIP, który trzeba rozpakować w całości w dowolnym miejscu. Paczka jest dostarczana razem z potrzebnymi bibliotekami oraz pakietem MikTeX Portable.

Głównym plikiem aplikacji jest **RaportPKUPGenerator.exe** (wyróżniony przy pomocy ikony z zielonym tekstem PKUP), który należy uruchomić poprzez podwójne kliknięcie lub zaznaczenie i przyciśnięcie *Enter* na klawiaturze.
