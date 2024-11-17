Projet Integrateur 1     17/04/2024 09:00

Auteurs : 
Souhayl Sejjari
Sebastian Cristescu
Mohamed Omar Zedek
Yassir Merzouki

Robot 1:
Branchements: ()

Comportement:
    Initialement, le robot commence par écrire certains éléments de dessin dans la mémoire externe. Nous avons choisi d'écrire tous les symboles nécessaires pour le dessin dans la mémoire externe afin d'éviter de saturer la mémoire flash, étant donné qu'elle contient déjà le programme à exécuter.

    Ensuite, nous implémentons une machine à états composée de 4 états, correspondant aux 4 principales étapes de la démonstration du robot 1, qui est démarré lorsque le bouton Interrupt est appuyé:

    État Positionnement:
        Il commence par faire une mesure de la distance pour pouvoir savoir à quel endroit le robot est positionné par rapport au mur. Celle-ci est rentré dans la fonction map qui prend une valeur
        entre 0x15 <-> 0x60/0x70 et retourne une valeur entre 1 <-> nLignesCadreRobot1 - 6 ou 6 <-> nLignesCadreRobot1 - 1 dépendamment de si le robot pointe vers
        le bas ou vers le haut. Cela vient de comment le dessin est implementé et il prend des valeurs différents selon si le robot pointe vers le haut ou le bas.

        Puis, il se met à tourner vers la droite en donnant des petit coup aux moteurs et en mesurant la distance. Celui-ci s'arrete lorsqu'il voit le trou, soit, lorsqu'il voit une
        distance < 0x15. Le programme compte combien de coup il donne et cela permet de savoir si le robot a fait un quart de tour avant de voir le trou ou trois-quart. En principe, si 
        le nombre de cycles est plus petit que 30 alors il a fait un quart de tour et il est positionné vers le haut. Dans l'autre cas, il a fait trois-quarts de tours
        donc il est orienté vers le bas. Le robot risque parfois de faire un tour extra sur lui-meme avant de trouver le trou, donc avant de sortir de la fonction, je m'assure de l'enlever.


    État ModeAttente:
        Lorsqu'il rentre dans cet état, il va faire un bruit sonore lorsque le bouton Interrupt est appuyé puis il va automatiquement passer au prochain état (Réception).


    État Réception:
        Cet etat permet de recuperer les informations dans des tableaux qui permettront ensuite de realiser une reproduction du parcours. Cet etat appelle la fonction recupererDonnee qui prend
        un tableau en prametre et y met la totalite des bits de chacune des donnees transmises (une a la fois), ensuite, un traitement sur ce tableau est fait avec les fonctions valeurDonnee, crcDonnee et verifierDonnee.
        Cette operation se repete autant de fois qu'il peut y avoir d'informations au maximum soit 18, nous mettons ensuite a l'aide de boucles for les informations correctes dans les bons tableaux afin d'ajouter les informations 
        des bandes dans le dessin. on verifie ensuite la justesse des information recoltees afin de passer a l'etat suivant, sinon refaire l'etat actuel.  


    État TransmissionRS232:
        Dans cet état, il allume une led avec une fréquence de 5Hz et envoie le dessin au pc par RS-232.



Robot 2:

Branchements: (Broches: 1 - 8)
    Les capteurs du suiveur de ligne sont branchés sur le portA, sur les broches 1 à 5 de gauche à droite. Ils sont également reliés à un VCC et à une masse (GND).
    Les moteurs sont branchés sur le portB, avec les pinces enable sur les broches 4 et 5, et les pins de direction sur les broches 3 et 6.
    Le bouton est relié à la broche 8 du portC, ainsi qu'au VCC et à la masse de ce port.
    La LED infrarouge est reliée à la broche 6 du portD et à sa masse. Enfin, la LED est branchée sur les broches 5 et 7 du portD.

Comportement:
    Initialement, le robot commence par écrire certains éléments de dessin dans la mémoire externe. Nous avons choisi d'écrire tous les symboles nécessaires pour le dessin dans la mémoire externe afin d'éviter de saturer la mémoire flash, étant donné qu'elle contient déjà le programme à exécuter.

    Ensuite, nous implémentons une machine à états composée de 4 états, correspondant aux 4 principales étapes de la démonstration du robot 2:

    État Parcours:
        Dans cet état, le robot 2 effectue plusieurs tâches liées au suivi de ligne sur le circuit avec les bandes.
        
        "performerParcours": Tout d'abord, il parcourt le circuit en suivant la ligne, en mesurant les longueurs des bandes rencontrées et en enregistrant leur position.
            La méthode performerParcours est une machine à états qui prend en entrée les signaux du capteur de suivi de ligne pour gérer les différents événements pendant le parcours. Pendant une ligne droite, le robot attend de rencontrer un point de rotation tout en restant sur la ligne. Lorsqu'il rencontre un point de rotation, il avance légèrement et détermine s'il s'agit d'une bande ou d'un virage. S'il détecte une bande, il aligne ses roues dessus, effectue une rotation jusqu'à ce qu'une durée prédéfinie soit atteinte ou jusqu'à ce qu'il retrouve la bande. S'il ne rencontre pas la bande, il retourne sur le parcours. Enfin, s'il détecte un point de rotation vers la gauche, il sait que c'est la fin du parcours, il aligne ses roues, pivote et se positionne face à la sortie de la boîte.
        
        "gererDonnees": Cette fonction traite les données enregistrées pendant le parcours. Son objectif principal est d'adapter ces données à la classe "dessin" afin de placer correctement les bandes sur le dessin.
        
        "ajouterBandesCadres": Cette fonction écrit dans la mémoire toutes les informations relatives aux bandes supplémentaires dans le circuit.

        Après ces tâches, le robot passe en mode "Attente", et donc à l'état Attente.

    État Attente:
        C'est un état où le robot attend deux événements spécifiques pour changer d'état. S'il détecte que l'utilisateur appuie sur le bouton blanc, il passe en mode EnvoiRS232. En revanche, s'il détecte que l'utilisateur appuie sur le bouton poussoir, il commence la transmission infrarouge. Dans les deux cas, après avoir effectué les opérations correspondantes, il revient à l'état "Attente".

    État EnvoiIR:
        Après avoir émis un son aigu, le robot commence l'émission infrarouge avec la fonction "emettreDonnees":
        "emettreDonnees": Cette méthode initialise les registres relatifs à l'émission infrarouge en utilisant le timer1 pour produire une onde CTC de fréquence 38000 Hz grâce à la fonction initialiserTransmission. Ensuite, les informations contenues dans les tableaux attributs sont envoyées donnée par donnée grâce à la fonction genererSequence, qui prend une valeur en uint8_t, afin de générer une onde infrarouge de démarrage, les bits de données sur 7 bits, et enfin un CRC sur 5 bits. Les valeurs sont alors transmises une à une accompagnées d'un CRC, faisant un total de 12 bits.

    État EnvoiRS232:
        Dans cet état, le robot lit la mémoire externe pour afficher le dessin complet tout en faisant clignoter la LED en vert. Après cette opération, il revient en mode attente.

