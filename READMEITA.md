CAN Validator
Un sistema C++23 per validazione e test di comunicazioni CAN bus utilizzando il pattern Adapter per interfacciarsi con dispositivi PCAN.
Caratteristiche

Pattern Adapter: Implementazione pulita per interfacciarsi con diversi dispositivi CAN
Serializzazione/Deserializzazione segnali: Gestione automatica di segnali CAN con mapping bit-level
Sistema di test modulare: Framework per creare e eseguire test automatizzati
Logging avanzato: Sistema di logging thread-safe con rotazione automatica dei file
Threading: Gestione separata di lettura/scrittura CAN in thread dedicati

Architettura
Componenti Principali

canAdapter: Classe base astratta che definisce l'interfaccia per la comunicazione CAN
pCanAdaptee: Implementazione specifica per dispositivi PCAN USB
canSignal.h: Definizioni di messaggi e segnali CAN con mapping preciso
Sistema di Test: Framework modulare per test di validazione
Logger: Sistema di logging thread-safe con supporto per diversi livelli

Pattern Utilizzati

Adapter Pattern: Per interfacciarsi con diversi tipi di dispositivi CAN
Singleton Pattern: Per il logger e il test dispatcher
RAII: Gestione automatica delle risorse

Requisiti

Compiler: C++23 compatibile
Sistema Operativo: Windows (per PCAN)
Hardware: Dispositivo PCAN USB
Librerie: PCANBasic (inclusa)

Build
Il progetto utilizza CMake:
bashmkdir build
cd build
cmake ..
cmake --build .
Dipendenze

PCANBasic.lib - Libreria PCAN (inclusa in /Pcan)
PCANBasic.dll - DLL PCAN (copiata automaticamente)

Utilizzo
Esempio Base
cpp#include "pCanAdaptee.h"
#include "canSignal.h"

int main() {
    // Crea adapter per PCAN
    std::unique_ptr<canAdapter> can = std::make_unique<pCanAdaptee>();
    
    // Invia un segnale CAN
    uint8_t value = 0xFF;
    can->setCanSignal(
        canSignals::messagesTx.at(canSignals::messageName::BYTETESTTX),
        canSignals::messagesTx.at(canSignals::messageName::BYTETESTTX)
            .signals.at(canSignals::signalName::BYTETESTBIT0),
        value
    );
    
    // Legge un segnale CAN
    uint8_t received = can->getCanSignal(
        canSignals::messagesRx.at(canSignals::messageName::BYTETESTRX),
        canSignals::messagesRx.at(canSignals::messageName::BYTETESTRX)
            .signals.at(canSignals::signalName::BYTETESTBIT0)
    );
    
    return 0;
}
Configurazione Segnali CAN
I segnali sono definiti in canSignal.h con strutture precise:
cppstruct canSignalMap_t {
    uint8_t  bitId;      // Posizione bit nel byte
    uint8_t  byteId;     // Posizione byte nel messaggio
    uint16_t sizeInBit;  // Dimensione in bit del segnale
};

struct canMessageMap_t {
    uint32_t messageId;  // ID del messaggio CAN
    uint8_t dlc;        // Data Length Code
    bool extended;      // Frame esteso (29-bit ID)
    std::unordered_map<signalName, canSignalMap_t> signals;
};
Sistema di Test
Esempio di Test (ByteTest)
Le classi test e ByteTest sono fornite come esempio di utilizzo del sistema. Mostrano come:

Creare test personalizzati ereditando da test
Utilizzare il testDispatcher per eseguire test in sequenza
Validare comunicazioni CAN bidirezionali

cpp#include "testDispatcher.h"
#include "testByte.h"

int main() {
    testDispatcher* dispatcher = testDispatcher::getInstance();
    
    // Aggiunge test alla coda
    dispatcher->addTest<ByteTest>();
    
    // Esegue tutti i test
    dispatcher->executeTestsInQueue();
    
    return 0;
}
Creare Test Personalizzati
Per creare nuovi test, eredita dalla classe test:
cppclass MioTest : public test {
public:
    void executeTest() override {
        setTestName("Il Mio Test");
        setTestResult(testResult::RUNNING);
        
        // Logica del test...
        
        setTestResult(testResult::PASS); // o FAIL
    }
};
Logging
Il sistema include un logger thread-safe con rotazione automatica:
cpp#include "logger.h"

// Logging base
Utils::Logger::log(Utils::Logger::logLevel_t::INFO) << "Messaggio" << std::endl;

// Logging con informazioni sul chiamante
Utils::Logger::log(Utils::Logger::logLevel_t::WARNING, LOGINVOKER) 
    << "Attenzione!" << std::endl;
Livelli di Log

INFO: Informazioni generali
DEBUG: Informazioni di debug
WARNING: Avvisi
CRITICAL: Errori critici

Struttura del Progetto
CAN_validator/
├── serializzazione_segnali/
│   ├── canAdapter.h/cpp          # Classe base adapter
│   ├── canSignal.h               # Definizioni segnali CAN
│   └── adaptee/
│       └── pCanAdaptee.h/cpp     # Implementazione PCAN
├── utils/
│   ├── logger/                   # Sistema di logging
│   └── generalUtils/             # Utilità generali
├── test/                         # Framework di test (esempi)
│   ├── test.h/cpp               # Classe base test (ESEMPIO)
│   ├── testDispatcher.h/cpp     # Dispatcher test
│   └── canTest/tests/
│       └── testByte.h/cpp       # Test esempio (ESEMPIO)
├── Pcan/                        # Librerie PCAN
└── CMakeLists.txt
Note

Thread Safety: Tutti i buffer CAN sono protetti da mutex
Gestione Errori: Eccezioni per errori di connessione e segnali inesistenti
Memory Management: Uso di smart pointer per gestione automatica memoria
Portabilità: Facilmente estendibile per altri dispositivi CAN

Esempi Forniti
Le seguenti classi sono fornite come esempi di utilizzo:

test: Classe base per implementare test personalizzati
ByteTest: Esempio concreto di test di comunicazione CAN
testDispatcher: Sistema per organizzare ed eseguire test in sequenza

Questi esempi mostrano le best practice per utilizzare il sistema e possono essere utilizzati come template per implementazioni specifiche.
Contributi
Feel free to contribute! Il codice è progettato per essere estensibile e modulare.
Licenza
MIT License - Vedi il file LICENSE per i dettagli.

Autore: Vincenzo Ziccardi
Email: vincenzoziccardi19@gmail.com
Italia, 2025
