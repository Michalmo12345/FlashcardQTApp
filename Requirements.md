# Wymagania

## Ubuntu 22.04 LTS
## FlashcardAPP

### Pobieranie zewnętrznych bibliotek
W celu pobrania bibliotek potrzebnych do prawidłowego funkcjonowania aplikacji, czyli
- Qt, do przedstawienia graficznego interfejsu użytkownika
- pq, PostgreSql do zarządzania bazą danych
należy nadać skryptowi uprawnienia do wykonania komendą:  

```chmod +x setup.sh```
a następnie odpalić go za pomocą
```./setup.sh```
Skrypt ten zainstaluje wymagane biblioteki lokalnie w katalogu projektu.

### Uruchamianie i konfiguracja bazy 
Docelowo baza danych będzie znajdowała się na serwerze. Aktualnie możemy uruchomić ją lokalnie w kontenerze.
Za pierwszym uruchomineiem należy wywołać polecenie  

`./init-dockers.sh`
Następnie odpalić bazę danych w kontenerze dockerowym komendą  

```docker compose up -d```

Przy pierwszym uruchomieniu możemy stworzyć tabele w bazie danych za pomocą polecenia:


```docker-compose exec db psql -U admin -d postgres -f /docker-entrypoint-initdb.d/init.sql```

Aby zatrzymać bazę danych wywołujemy komendę  

```docker compose down```


**Adminer**
Adminer to narzędzie do zarządzania zawartością baz danych. Możesz go uruchomić pod adresem http://localhost:9001
### Budowanie i kompilacja
Do kompilacji używamy narzędzia cmake. Należy upewnić się że jest ono pobrane, jeśli nie - pobieramy komendą:  

```sudo apt-get install cmake```

CMakeLists pozwala na dwa tryby budowania:
- budowanie aplikacji
- budowanie testów
Domyślnie ustawione jest budowanie aplikacji. 
W celu zmiany trybu musimy wywołać komendę

`cmake . -DBUILD_TESTS=ON' 

gdy chcemy budować testy
i odpowiednio:
`cmake . -DBUILD_TESTS=OFF' do budowania aplikacji
Następnie wywołujemy komendę:
cmake --build.
Następnie będąc w katalogu w którym znajduje się aplikacja odpalamy aplikacje komendą  

```./FlashcardApp```  

lub w przypadku testów   

```tests\tests```