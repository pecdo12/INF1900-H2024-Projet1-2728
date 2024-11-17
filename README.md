## INF1900-H2024-Projet1-2728
Projet 1 de génie logiciel à Polytechnique Montréal pour le cours INF1900. Le repertoire contient les fichiers de laboratoire des deux équipes ainsi que les fichiers du projet final.

# Objectif Principal du Projet Intégrateur
L'objectif principal de ce projet est de concevoir, programmer et intégrer deux robots collaboratifs capables de réaliser une série de tâches sur une table délimitée, incluant la détection et la mesure de segments ajoutés, la communication de données via infrarouge (IR) et RS-232, ainsi que la génération d'un dessin cartographique des éléments identifiés sur un ordinateur. Ce projet met l'accent sur le respect des requis techniques, l'utilisation judicieuse des technologies, et la gestion des erreurs de transmission. Pour plus d'informations, le document de consignes est joint.

# Différentes technologies utilisées dans le cadre de ce projet
1. Suivi de ligne : Utilisation du capteur Maker Line pour permettre au Robot 2 de suivre les lignes noires tracées sur la table et détecter les segments ajoutés.
2. Communication infrarouge (IR) : Implémentation du protocole SIRC pour la transmission des données entre les deux robots. Cela inclut l'envoi et la réception fiables avec la vérification des erreurs (CRC-16).
3. RS-232 : Transmission de données du robot vers un PC via un port série pour afficher le dessin cartographique des éléments identifiés.
4. PWM (Pulse Width Modulation) : Utilisé pour contrôler la vitesse des moteurs des robots et la précision de leurs mouvements.
5. Capteurs de distance : GP2Y0A21YK0F pour aider à déterminer la position et les déplacements du Robot 1.
6. Éclairage et signaux visuels : Les LED indiquent les différents états des robots (par exemple, attente, réception ou envoi de données).


# Remerciements
J'aimerais remercier Mohamed Omar Zedek et Souhayl Sejjari pour leurs contributions indispensables à la réalisation de ce projet. Grâce à nos efforts, ce projet à été un véritable succès.
