const int BOUTON_PIN = 2;
const int LED_PINS[] = {8, 9, 10, 11}; // Broches des LEDs
const int POTENTIOMETRE_PIN = A1;
const unsigned long DELAI_ANTI_REBOND = 50; // Anti-rebond bouton
const int VALEUR_MIN = 0;
const int VALEUR_MAX = 20;
const int POURCENTAGE_MAX = 100;
const int NB_SYMBOLS_MAX = 20;
const int NB_LEDS = 4;
const int ANALOG_MAX = 1023;

int valeurPotentiometre = 0;
int valeurMappee = 0;
bool dernierEtatBouton = HIGH; // Pour detecter l'appui du bouton
unsigned long dernierTempsDebounce = 0;
int pourcentage = 0;
int nombreSymboles = 0;
int indexLed = 0;
int etatBouton = 0;

void initialiserMateriel() {
    Serial.begin(9600);
    pinMode(BOUTON_PIN, INPUT_PULLUP);
    
    for (int i = 0; i < NB_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);
    }
}

int lirePotentiometre() {
    valeurPotentiometre = analogRead(POTENTIOMETRE_PIN);
    valeurMappee = map(valeurPotentiometre, VALEUR_MIN, ANALOG_MAX, VALEUR_MIN, VALEUR_MAX);
    return valeurMappee;
}

bool boutonAppuye() {
    etatBouton = digitalRead(BOUTON_PIN);
    if (etatBouton == LOW && dernierEtatBouton == HIGH && millis() - dernierTempsDebounce > DELAI_ANTI_REBOND) {
        dernierTempsDebounce = millis();
        dernierEtatBouton = etatBouton;
        return true;
    }
    dernierEtatBouton = etatBouton;
    return false;
}

// Fonction pour afficher la barre de progression
void afficherBarreProgression(int valeur) {
    pourcentage = map(valeur, VALEUR_MIN, VALEUR_MAX, VALEUR_MIN, POURCENTAGE_MAX);
    nombreSymboles = map(valeur, VALEUR_MIN, VALEUR_MAX, VALEUR_MIN, NB_SYMBOLS_MAX); // Nombre de '@' a afficher

    Serial.print(pourcentage);
    Serial.print("% [");
    
    for (int i = 0; i < NB_SYMBOLS_MAX; i++) {
        Serial.print(i < nombreSymboles ? "@" : ".");
    }
    Serial.println("]");
}

// Fonction pour gerer les LEDs en fonction de la valeur du potentiometre
void gererLeds(int valeur) {
    indexLed = map(valeur, VALEUR_MIN, VALEUR_MAX, VALEUR_MIN, NB_LEDS);
    
    for (int i = 0; i < NB_LEDS; i++) {
        digitalWrite(LED_PINS[i], (i == indexLed) ? HIGH : LOW);
    }
}

void setup() {
    initialiserMateriel();
}

void loop() {
    valeurMappee = lirePotentiometre();
    
    if (boutonAppuye()) {
        afficherBarreProgression(valeurMappee);
    }
    
    gererLeds(valeurMappee);
}
