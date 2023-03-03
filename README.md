# Piano MIDI contrôlé par microcontrôleur

    ## Objectif : 
    utiliser un microcontrôleur Arduino Leonardo pour convertir les messages MIDI en signaux de contrôle pour activer les électroaimants correspondants du piano
    
    #### Contrainte : 
    - le système ne gère aucun parametresmidi, il ne fait que réagir aux messages noteOn et noteOff
    - le systeme lit et execute tout les canaux midi recu

    ## Materiel utilisés :

    - Arduino Leonardo
    - 8 x MCP23017
    - 88 électroaimants contrôlés par les ULN2803 ou ULN2804

    ## Fonctionnement :

    Les messages MIDI sont convertis en signaux de contrôle pour activer les électroaimants correspondants du piano
    L'ordre des notes sur le piano suit l'ordre des sorties des MCP23017 et la première note du piano est le numéro MIDI 21
    Lorsqu'un message MIDI est reçu, il est analysé pour déterminer la note à jouer
    Le code utilise une interruption matérielle pour gérer la temporisation et jouer les notes plus rapidement
