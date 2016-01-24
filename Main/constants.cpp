/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "constants.h"

// Localization
#define LOCALE_ENGLISH

// Programm allgemein
extern const char* STR_APP_NAME				= "PecoRename";

/*********************************************************************************************
*	ITALIAN
**********************************************************************************************/

#ifdef LOCALE_ITALIAN

// general
extern const char* STR_APP_VERSION			= "Release 1.5";
extern const char* HELP_FILE 				= "documentation/index.html";

extern const char* STR_YES					= "Si";
extern const char* STR_NO					= "No";
extern const char* STR_OK					= "Ok";
extern const char* STR_WELL					= "Bene...";
// This are the time formats. Just tell me which format you use for the date.
// I.e. the date format in Germany is 02.12.2000 for December 12, 2000.
extern const char* STR_DATE_TIME_SECS_FORMAT	= "%d/%m/%Y, %T";//"%x, %r";
extern const char* STR_DATE_TIME_FORMAT		= "%d/%m/%Y, %H:%M";//"%x, %I:%M %p";
extern const char* STR_DATE_FORMAT			= "%d/%m/%Y";//"%x";
extern const char* STR_CANCEL				= "Annulla";
extern const char* STR_CONTINUE				= "Continua";
extern const char* STR_CONTINUE_WO_MSG		= "Continua senza messaggi";

// Panel
extern const char* STR_PANEL_TITLE			= "Seleziona i files da rinominare";
extern const char* STR_SELECT				= "Seleziona";

// Menu
extern const char* STR_MENU_FILE			= "File";
extern const char* STR_MENU_NEW 			= "Nuovo";
extern const char* STR_MENU_OPEN 			= "Seleziona files...";
extern const char* STR_MENU_ABOUT			= "Informazioni...";
extern const char* STR_MENU_QUIT			= "Esci";

extern const char* STR_MENU_TOOLS			= "Strumenti";
extern const char* STR_MENU_CREATE_SCRIPT	= "Crea script per la shell...";

extern const char* STR_MENU_HELP			= "Aiuto";
extern const char* STR_MENU_DOCU			= "Documentazione";

//StatusBar (Messages)
extern const char* STATUS_STATUS			= "Stato: ";
extern const char* STATUS_SELECT_FILES		= "Seleziona i files!";
extern const char* STATUS_IMPORT			= "Importazione...";
extern const char* STATUS_SELECT_MODE		= "Seleziona una modalità di ridenominazione";
extern const char* STATUS_SORTING			= "Ordinamento...";
extern const char* STATUS_PREVIEW			= "Creazione anteprima...";
extern const char* STATUS_CHECKDUPS			= "Ci sono problemi?";
extern const char* STATUS_RENAMING			= "Ridenominazione...";

extern const char* STATUS_WAITING			= "Aspetto istruzioni... :-)";
extern const char* STATUS_DIDIT_FINE		= "I feel good!";

extern const char* STATUS_DIDIT_BAD			= "Si sono verificati errori.";

// TopView
extern const char* STR_TOPVIEW_TITLE		= "Seleziona i files e le cartelle...";

extern const char* STR_PLEASE_CHOOSE		= "Seleziona...";
extern const char* STR_PATH					= "Percorso:";
extern const char* STR_NAME					= "Nome";
extern const char* STR_SIZE					= "Dimensioni";
extern const char* STR_DATE					= "Modificato";
extern const char* STR_PREVIEW				= "Anteprima";

// BottomView
extern const char* STR_BOTTOMVIEW_TITLE		= "...e rinominali!";
extern const char* STR_MODE					= "Modo:";
extern const char* STR_PLEASE_SELECT		= "Seleziona";

// More GUI elements
extern const char* STR_DO_IT				= "Si parte!";
extern const char* STR_ABOUT_TITLE			= "Informazioni su PecoRename";
extern const char* STR_ABOUT_COPYRIGHT_0	= "Copyright ©2000 by Werner Freytag";
extern const char* STR_ABOUT_COPYRIGHT_1	= "Questo programma è freeware.";
extern const char* STR_ABOUT_COPYRIGHT_2	= "Grazie a Gian Davide per la traduzione italiana!";
extern const char* STR_ABOUT_THANKYOU		= "Grazie :-)"; // For reading the About window

// Errormessages
extern const char* MESSAGE_MULTIDIR			= "Spiacente, ma non posso rinominare files da cartelle diverse.\n\n"
											  "Soltanto i files nella prima cartella trovata saranno importati!";

extern const char* MESSAGE_ERROR_IN_REGEX	= "C'è un errore nell'espressione regolare...";

extern const char* MESSAGE_WILL_HAVE_PROBS	= "Mi aspetto qualche problema con i nomi usati due volte.\n\n"
											  "Avvio ugualmente il processo?";

extern const char* MESSAGE_HAVE_PROBLEM		= "Si è verificato un problema rinominando '%1' in '%2'.\n\n"
											  "Si desidera annullare, continuare oppure continuare senza ricevere ulteriori messaggi di errore?";

extern const char* MESSAGE_MARKED_FILES		= "Ho segnato in rosso i files che hanno generato un errore!";

extern const char* MESSAGE_REALLY_DOIT		= "Si desidera rinominare il file?\nQuesto potrebbe generare degli errori!\n\n"
											  "Se si clicca su 'Continua', i files verranno rinominati a VOSTRO RISCHIO!";
extern const char* MESSAGE_NOTHING_TO_DO	= "Che faccio?";

extern const char* MESSAGE_NOHELP			= "Si è verificato un errore:\nNon riesco a trovare il file della guida, oppure non posso trovare un browser HTML.";

// Renamer names
extern const char* REN_SEARCH_REPLACE		= "Cerca e sostituisci";
extern const char* REN_NUMBERING			= "Numerazione";
extern const char* REN_EXTENSION			= "Aggiungi l'estensione al file";
extern const char* REN_INSERTREPLACE		= "Inserisci / sostituisci";
extern const char* REN_UPPERLOWER			= "Maiuscolo / minuscolo";
extern const char* REN_REMOVE				= "Rimuovi";

// Renamer settings

// Search & Replace
extern const char* REN_SET_REGEX			= "Espressione regolare";
extern const char* REN_SET_MATCHCASE		= "Considera maiuscole/minuscole";
extern const char* REN_SET_SEARCHPATTERN	= "Trova pattern:";
extern const char* REN_SET_REPLACESTRING	= "Sostituisci con:";

// File extension
extern const char* REN_SET_EXT_REPLACE_OLD	= "Sostituisci la vecchia estensione";
extern const char* REN_SET_EXT_UPPERLOWER	= "Maiuscolo / minuscolo:";
extern const char* REN_SET_DEFAULT			= "Default";
extern const char* REN_SET_LOWERCASE		= "minuscolo";
extern const char* REN_SET_UPPERCASE		= "MAIUSCOLO";

// Numbering
extern const char* REN_SET_FORMAT			= "Formato:";
extern const char* REN_SET_STARTWITH		= "Comincia con:";
extern const char* REN_SET_TEXTBEFORE		= "Testo prima:";
extern const char* REN_SET_TEXTBEHIND		= "Testo dopo:";

// Insert / replace
extern const char* REN_SET_INSERTREPLACE	= "Inserisci o sostituisci:";
extern const char* REN_SET_INSERT			= "Inserisci";
extern const char* REN_SET_REPLACE			= "sostituisci con";
extern const char* REN_SET_TEXT				= "Testo:";
extern const char* REN_SET_ATPOSITION		= "Nella posizione:";
extern const char* REN_SET_FROMLEFT			= "dall'inizio (sinistra)";
extern const char* REN_SET_FROMRIGHT		= "dalla fine (destra)";

// Uppercase / lowercase
extern const char* REN_SET_CONVERTTO		= "Converti in";

// Remove
extern const char* REN_SET_FROMPOS			= "Rimuovi i caratteri dalla posizione";
extern const char* REN_SET_TOPOS			= "alla posizione";

#endif /* LOCALE_ITALIAN */


/*********************************************************************************************
*	FRANZÖSISCH
**********************************************************************************************/

#ifdef LOCALE_FRENCH

// general
extern const char* STR_APP_VERSION			= "Version 1.5";
extern const char* HELP_FILE 				= "documentation/index.html";

extern const char* STR_YES					= "Oui";
extern const char* STR_NO					= "Non";
extern const char* STR_OK					= "Ok";
extern const char* STR_WELL					= "Oups...";
extern const char* STR_DATE_TIME_SECS_FORMAT	= "%d/%m/%Y, %T";
extern const char* STR_DATE_TIME_FORMAT		= "%d/%m/%Y, %H:%M";
extern const char* STR_DATE_FORMAT			= "%d/%m/%Y";

extern const char* STR_CANCEL				= "Annuler";
extern const char* STR_CONTINUE				= "Continuer";
extern const char* STR_CONTINUE_WO_MSG		= "Continuer sans messages";

// Panel
extern const char* STR_PANEL_TITLE			= "Sélection des fichiers à renommer";
extern const char* STR_SELECT				= "Séletionner";

// Menu
extern const char* STR_MENU_FILE			= "Fichier";
extern const char* STR_MENU_NEW 			= "Nouveau";
extern const char* STR_MENU_OPEN 			= "Sélectionner les fichiers...";
extern const char* STR_MENU_ABOUT			= "A propos...";
extern const char* STR_MENU_QUIT			= "Quitter";

extern const char* STR_MENU_TOOLS			= "Outils";
extern const char* STR_MENU_CREATE_SCRIPT	= "Créer un script shell...";

extern const char* STR_MENU_HELP			= "Aide";
extern const char* STR_MENU_DOCU			= "Documentation";

//StatusBar
extern const char* STATUS_STATUS			= "Status : ";
extern const char* STATUS_SELECT_FILES		= "Sélectionner les fichiers !";
extern const char* STATUS_IMPORT			= "Préparation...";
extern const char* STATUS_SELECT_MODE		= "Sélectionner un mode de renommage !";
extern const char* STATUS_SORTING			= "Tri...";
extern const char* STATUS_PREVIEW			= "Création de l'aperçu...";
extern const char* STATUS_CHECKDUPS			= "Y a-t-il des problèmes ?";
extern const char* STATUS_RENAMING			= "Renommage en cours...";
extern const char* STATUS_WAITING			= "A vos ordres... :-)";
extern const char* STATUS_DIDIT_FINE		= "Renommage réussi";
extern const char* STATUS_DIDIT_BAD			= "Erreurs détectées.";

// TopView
extern const char* STR_TOPVIEW_TITLE		= "Sélectionnez vos fichiers et répertoires...";

extern const char* STR_PLEASE_CHOOSE		= "Sélectionner...";
extern const char* STR_PATH					= "Répertoire :";
extern const char* STR_NAME					= "Nom";
extern const char* STR_SIZE					= "Taille";
extern const char* STR_DATE					= "Modifié le";
extern const char* STR_PREVIEW				= "Aperçu";

// BottomView
extern const char* STR_BOTTOMVIEW_TITLE		= "...et renommez-les :";
extern const char* STR_MODE					= "Mode:";
extern const char* STR_PLEASE_SELECT		= "Sélectionner";

// More GUI elements
extern const char* STR_DO_IT				= "C'est parti !";	// was "Let's do it!"
extern const char* STR_ABOUT_TITLE			= "A propos de PecoRename";
extern const char* STR_ABOUT_COPYRIGHT_0	= "Copyright ©2000, Werner Freytag";
extern const char* STR_ABOUT_COPYRIGHT_1	= "Ce logiciel est freeware.";
extern const char* STR_ABOUT_COPYRIGHT_2	= "Merci à Stéphane pour la traduction française.";
extern const char* STR_ABOUT_THANKYOU		= "Merci :-)";

// Errormessages
extern const char* MESSAGE_MULTIDIR			= "Désolé, impossible de renommer des fichiers provenant de différents répertoire\n\n//"
											  "Seuls les fichiers suivants (provenant du premier répertoire trouvé) seront importés\n\n";
												// I'm sorry, but I can't rename files from different directories.\n\n
												// Only those files will be imported, which are in the first found directory!";
extern const char* MESSAGE_ERROR_IN_REGEX	= "Expression régulière incorrecte"; //"There is an error in the regular expression...";
extern const char* MESSAGE_WILL_HAVE_PROBS	= "Risque d'erreur du à l'utilisation multiple d'un nom de fichier"
											  "Voulez-vous procéder au renommage ?";
												//"I'm expecting problems with double used file names.\n\n
												//Shall I still start with the renaming?";
extern const char* MESSAGE_HAVE_PROBLEM		= "Erreur lors du renommage de '%1' en '%2'.\n\n"
											  "Abandonner, continer, continuer sans aucun message d'erreur ?" ;
												// "There occured a problem when renaming '%1' to '%2'.\n\n
												//Do you want to cancel, continue or continue without getting any further error messages?";
extern const char* MESSAGE_MARKED_FILES		= "Les fichiers ayant provoqué une erreur ont été marqué en rouge"; // "I've marked those files, which caused the errors, in red!";
extern const char* MESSAGE_REALLY_DOIT		= "Voulez-vous vraiment renommer ces fichiers ?\n"
											  "Cette opération peut provoquer des problèmes !\n\n"
											  "Choisissez 'Continuer' (A VOS RISQUES ET PERILS) ou annulez !";
												//"Do you really want to rename these files?
												//\nThis could probably lead to problems!\n\n
												//If you click on 'Continue', the files will be renamed AT YOUR OWN RISK!";
extern const char* MESSAGE_NOTHING_TO_DO	= "Que dois-je faire ?"; // (not very good) "What shall I do?";
extern const char* MESSAGE_NOHELP			= "Erreur : fichier d'aide manquant ou aucun navigateur trouvé";
												//"There has occured an error:\n
												//Either the help file is missing, or a browser to show it.";

// Renamer names
extern const char* REN_SEARCH_REPLACE		= "Rechercher / Remplacer"; // "Search and replace";
extern const char* REN_NUMBERING			= "Numérotation"; //"Numbering";
extern const char* REN_EXTENSION			= "Ajouter une extension de fichier" ; // "Add file extension";
extern const char* REN_INSERTREPLACE		= "Insertion / Remplacement"; //"Insert / replace";
extern const char* REN_UPPERLOWER			= "Majuscule / Minuscule"; //"Uppercase / lowercase";
extern const char* REN_REMOVE				= "Supprimer"; //"Remove";

// Renamer settings
extern const char* REN_SET_REGEX			= "Expression régulière"; //"Regular expression";
extern const char* REN_SET_MATCHCASE		= "Différentiation MAJ/min" ; // "Case sensitive";
extern const char* REN_SET_SEARCHPATTERN	= "Rechercher :"; // can pattern be omitted "Find pattern:";
extern const char* REN_SET_REPLACESTRING	= "Remplacer par :"; // "Replace with:";

extern const char* REN_SET_EXT_REPLACE_OLD	= "Remplacer l'ancienne extension"; //"Replace old extension";
extern const char* REN_SET_EXT_UPPERLOWER	= "Majuscule / Minuscule"; //"Uppercase / lowercase:";
extern const char* REN_SET_DEFAULT			= "Defaut"; // (or "Par defaut"...) "Default";
extern const char* REN_SET_LOWERCASE		= "Minuscule"; //"lowercase";
extern const char* REN_SET_UPPERCASE		= "MAJUSCULE"; //"UPPERCASE";

extern const char* REN_SET_FORMAT			= "Format : ";
extern const char* REN_SET_STARTWITH		= "Commencer à : ";
extern const char* REN_SET_TEXTBEFORE		= "Texte en tête"; //"Text before:";
extern const char* REN_SET_TEXTBEHIND		= "Texte en fin"; //"Text behind:";

extern const char* REN_SET_INSERTREPLACE	= "Insérer ou remplacer";
extern const char* REN_SET_INSERT			= "Insérer";
extern const char* REN_SET_REPLACE			= "Remplacer par";
extern const char* REN_SET_TEXT				= "Texte :";
extern const char* REN_SET_ATPOSITION		= "Position :";
extern const char* REN_SET_FROMLEFT			= "en tête (à gauche)";
extern const char* REN_SET_FROMRIGHT		= "en fin (à droite)";

extern const char* REN_SET_CONVERTTO		= "Convertir en";

extern const char* REN_SET_FROMPOS			= "Supprime les caractères de la position";
extern const char* REN_SET_TOPOS			= "à la position";

#endif /* LOCALE_FRENCH */


/*********************************************************************************************
*	ENGLISH
**********************************************************************************************/

#ifdef LOCALE_ENGLISH

// general
extern const char* STR_APP_VERSION			= "Release 1.5";
extern const char* HELP_FILE 				= "documentation/index.html";

extern const char* STR_YES					= "Yes";
extern const char* STR_NO					= "No";
extern const char* STR_OK					= "Ok";
extern const char* STR_WELL					= "Well...";
extern const char* STR_DATE_TIME_SECS_FORMAT	= "%x, %r";
extern const char* STR_DATE_TIME_FORMAT		= "%x, %I:%M %p";
extern const char* STR_DATE_FORMAT			= "%x";
extern const char* STR_CANCEL				= "Cancel";
extern const char* STR_CONTINUE				= "Continue";
extern const char* STR_CONTINUE_WO_MSG		= "Continue without messages";

// Panel
extern const char* STR_PANEL_TITLE			= "Select files for renaming";
extern const char* STR_SELECT				= "Select";

// Menu
extern const char* STR_MENU_FILE			= "File";
extern const char* STR_MENU_NEW 			= "New";
extern const char* STR_MENU_OPEN 			= "Select files...";
extern const char* STR_MENU_ABOUT			= "About...";
extern const char* STR_MENU_QUIT			= "Quit";

extern const char* STR_MENU_TOOLS			= "Tools";
extern const char* STR_MENU_CREATE_SCRIPT	= "Create shell script...";

extern const char* STR_MENU_HELP			= "Help";
extern const char* STR_MENU_DOCU			= "Documentation";

//StatusBar
extern const char* STATUS_STATUS			= "Status: ";
extern const char* STATUS_SELECT_FILES		= "Please select files!";
extern const char* STATUS_IMPORT			= "Importing...";
extern const char* STATUS_SELECT_MODE		= "Please select a rename mode!";
extern const char* STATUS_SORTING			= "Sorting...";
extern const char* STATUS_PREVIEW			= "Creating preview...";
extern const char* STATUS_CHECKDUPS			= "Are there any problems?";
extern const char* STATUS_RENAMING			= "Renaming...";

extern const char* STATUS_WAITING			= "Awaiting your instructions... :-)";
extern const char* STATUS_DIDIT_FINE		= "I feel good!";

extern const char* STATUS_DIDIT_BAD			= "Errors occurred.";

// TopView
extern const char* STR_TOPVIEW_TITLE		= "Select your files and directories...";

extern const char* STR_PLEASE_CHOOSE		= "Please select...";
extern const char* STR_PATH					= "Path:";
extern const char* STR_NAME					= "Name";
extern const char* STR_SIZE					= "Size";
extern const char* STR_DATE					= "Modified";
extern const char* STR_PREVIEW				= "Preview";

// BottomView
extern const char* STR_BOTTOMVIEW_TITLE		= "...and rename them!";
extern const char* STR_MODE					= "Mode:";
extern const char* STR_PLEASE_SELECT		= "Please select";

// More GUI elements
extern const char* STR_DO_IT				= "Let's do it!";
extern const char* STR_ABOUT_TITLE			= "About PecoRename";
extern const char* STR_ABOUT_COPYRIGHT_0	= "Copyright ©2000 by Werner Freytag";
extern const char* STR_ABOUT_COPYRIGHT_1	= "This software is freeware.";
extern const char* STR_ABOUT_COPYRIGHT_2	= "Special thanks to Sci for his help with the English translation!";
extern const char* STR_ABOUT_THANKYOU		= "Thank you :-)";

// Errormessages
extern const char* MESSAGE_MULTIDIR			= "I'm sorry, but I can't rename files from different directories.\n\n"
											  "Only the files in the first found directory will be imported!";

extern const char* MESSAGE_ERROR_IN_REGEX	= "There is an error in the regular expression...";

extern const char* MESSAGE_WILL_HAVE_PROBS	= "I expect some problems with double used file names.\n\n"
											  "Should I still start with the renaming?";

extern const char* MESSAGE_HAVE_PROBLEM		= "A problem occurred when renaming '%1' to '%2'.\n\n"
											  "Do you want to cancel, continue or continue without getting any further error messages?";

extern const char* MESSAGE_MARKED_FILES		= "I've marked the files that caused the errors in red!";

extern const char* MESSAGE_REALLY_DOIT		= "Do you really want to rename these files?\nThis could probably lead to problems!\n\n"
											  "If you click on 'Continue', the files will be renamed AT YOUR OWN RISK!";
extern const char* MESSAGE_NOTHING_TO_DO	= "What shall I do?";

extern const char* MESSAGE_NOHELP			= "An error has occurred:\nEither the help file is missing, or an HTML browser can not be found.";

// Renamer names
extern const char* REN_SEARCH_REPLACE		= "Search and replace";
extern const char* REN_NUMBERING			= "Numbering";
extern const char* REN_EXTENSION			= "Add file extension";
extern const char* REN_INSERTREPLACE		= "Insert / replace";
extern const char* REN_UPPERLOWER			= "Uppercase / lowercase";
extern const char* REN_REMOVE				= "Remove";

// Renamer settings
extern const char* REN_SET_REGEX			= "Regular expression";
extern const char* REN_SET_MATCHCASE		= "Case sensitive";
extern const char* REN_SET_SEARCHPATTERN	= "Find pattern:";
extern const char* REN_SET_REPLACESTRING	= "Replace with:";

extern const char* REN_SET_EXT_REPLACE_OLD	= "Replace old extension";
extern const char* REN_SET_EXT_UPPERLOWER	= "Uppercase / lowercase:";
extern const char* REN_SET_DEFAULT			= "Default";
extern const char* REN_SET_LOWERCASE		= "lowercase";
extern const char* REN_SET_UPPERCASE		= "UPPERCASE";

extern const char* REN_SET_FORMAT			= "Format:";
extern const char* REN_SET_STARTWITH		= "Start with:";
extern const char* REN_SET_TEXTBEFORE		= "Text before:";
extern const char* REN_SET_TEXTBEHIND		= "Text behind:";

extern const char* REN_SET_INSERTREPLACE	= "Insert or replace:";
extern const char* REN_SET_INSERT			= "Insert";
extern const char* REN_SET_REPLACE			= "Replace with";
extern const char* REN_SET_TEXT				= "Text:";
extern const char* REN_SET_ATPOSITION		= "At position:";
extern const char* REN_SET_FROMLEFT			= "from the front (left)";
extern const char* REN_SET_FROMRIGHT		= "from the back (right)";

extern const char* REN_SET_CONVERTTO		= "Convert to";

extern const char* REN_SET_FROMPOS			= "Remove characters from position";
extern const char* REN_SET_TOPOS			= "to position";

#endif /* LOCALE_ENGLISH */


/*********************************************************************************************
*	DEUTSCH
**********************************************************************************************/

#ifdef LOCALE_DEUTSCH

// general
extern const char* STR_APP_VERSION			= "Version 1.5";
extern const char* HELP_FILE 				= "Anleitung/index.html";

// allgemein
extern const char* STR_YES					= "Ja";
extern const char* STR_NO					= "Nein";
extern const char* STR_OK					= "Ok";
extern const char* STR_WELL					= "Äh...";
extern const char* STR_DATE_TIME_SECS_FORMAT	= "%d.%m.%Y, %T";
extern const char* STR_DATE_TIME_FORMAT		= "%d.%m.%Y, %H:%M";
extern const char* STR_DATE_FORMAT			= "%d.%m.%Y";
extern const char* STR_CANCEL				= "Abbrechen";
extern const char* STR_CONTINUE				= "Fortfahren";
extern const char* STR_CONTINUE_WO_MSG		= "Fortfahren ohne Fehlermeldungen";

// Panel
extern const char* STR_PANEL_TITLE			= "Dateien zum Umbenennen wählen";
extern const char* STR_SELECT				= "Wählen";

// Menü
extern const char* STR_MENU_FILE			= "Datei";
extern const char* STR_MENU_NEW 			= "Neu";
extern const char* STR_MENU_OPEN 			= "Dateien wählen...";
extern const char* STR_MENU_ABOUT			= "Über...";
extern const char* STR_MENU_QUIT			= "Beenden";

extern const char* STR_MENU_TOOLS			= "Werkzeug";
extern const char* STR_MENU_CREATE_SCRIPT	= "Shell-Script erzeugen...";

extern const char* STR_MENU_HELP			= "Hilfe";
extern const char* STR_MENU_DOCU			= "Anleitung";

//StatusBar
extern const char* STATUS_STATUS			= "Status: ";
extern const char* STATUS_SELECT_FILES		= "Bitte Dateien wählen!";
extern const char* STATUS_IMPORT			= "Importiere...";
extern const char* STATUS_SELECT_MODE		= "Bitte Modus für das Umbenennen wählen!";
extern const char* STATUS_SORTING			= "Sortiere...";
extern const char* STATUS_PREVIEW			= "Erstelle die Vorschau...";
extern const char* STATUS_CHECKDUPS			= "Gibt's Probleme?";
extern const char* STATUS_RENAMING			= "Benenne um...";
extern const char* STATUS_WAITING			= "Erwarte Deine Anweisungen... :-)";
extern const char* STATUS_DIDIT_FINE		= "Alles bestens!";
extern const char* STATUS_DIDIT_BAD			= "Es sind Fehler aufgetreten.";

// TopView
extern const char* STR_TOPVIEW_TITLE		= "Wähle Deine Dateien und Verzeichnisse...";

//Buttons
extern const char* STR_PLEASE_CHOOSE		= "Bitte wählen...";
extern const char* STR_PATH					= "Pfad:";
extern const char* STR_NAME					= "Name";
extern const char* STR_SIZE					= "Größe";
extern const char* STR_DATE					= "Änderungsdatum";
extern const char* STR_PREVIEW				= "Vorschau";

// BottomView
extern const char* STR_BOTTOMVIEW_TITLE		= "...und benenne sie um!";
extern const char* STR_MODE					= "Modus:";
extern const char* STR_PLEASE_SELECT		= "Bitte wählen";

// Weitere GUI-Elemente
extern const char* STR_DO_IT				= "Los geht's!";
extern const char* STR_ABOUT_TITLE			= "Über PecoRename";
extern const char* STR_ABOUT_COPYRIGHT_0	= "Copyright ©2000 by Werner Freytag";
extern const char* STR_ABOUT_COPYRIGHT_1	= "Dieses Programm ist Freeware.";
extern const char* STR_ABOUT_COPYRIGHT_2	= "Ein großes Dankeschön an Stéphane und Sci für die Hilfe!";
extern const char* STR_ABOUT_THANKYOU		= "Dankeschön :-)";

// Fehlermeldungen
extern const char* MESSAGE_MULTIDIR			= 	"Leider können nicht Dateien aus verschiedenen Verzeichnissen umbenannt werden.\n\n"
												"Es werden nur die Dateien importiert, die sich im ersten übergebenen Verzeichnis befinden!";
extern const char* MESSAGE_ERROR_IN_REGEX	= 	"Im regulären Ausdruck befindet sich ein Fehler...";
extern const char* MESSAGE_WILL_HAVE_PROBS	= 	"Es wird voraussichtlich Probleme mit doppelt vorhandenen Dateinamen geben.\n\n"
												"Soll trotzdem mit dem Umbenennen begonnen werden?";
extern const char* MESSAGE_HAVE_PROBLEM		= 	"Es ist ein Problem beim Umbenennen von '%1' in '%2' aufgetreten.\n\n"
												"Willst Du abbrechen, fortfahren oder fortfahren, ohne weitere Fehlermeldungen angezeigt zu bekommen?";
extern const char* MESSAGE_MARKED_FILES		= 	"Ich habe die Dateien, die Fehler verursacht haben, rot markiert!";
extern const char* MESSAGE_REALLY_DOIT		= 	"Willst Du diese Dateien wirklich umbenennen?\nDies könnte möglicherweise zu Problemen führen!\n\n"
												"Wenn Du auf 'Fortfahren' klickst, werden die Dateien auf DEINE EIGENE VERANTWORTUNG umbenannt.";
extern const char* MESSAGE_NOTHING_TO_DO	= 	"Was soll ich denn tun?";
extern const char* MESSAGE_NOHELP			= 	"Es ist ein Fehler auftetreten:\nEntweder die Hilfedatei fehlt, oder ein Browser, um sie anzuzeigen.";

// Renamer-Namen
extern const char* REN_SEARCH_REPLACE		= "Suchen und ersetzen";
extern const char* REN_NUMBERING			= "Nummerieren";
extern const char* REN_EXTENSION			= "Dateiendung hinzufügen";
extern const char* REN_INSERTREPLACE		= "Einfügen / überschreiben";
extern const char* REN_UPPERLOWER			= "Groß- & Kleinschreibung";
extern const char* REN_REMOVE				= "Entfernen";

// Renamer-Settings
extern const char* REN_SET_REGEX			= "Regulärer Ausdruck";
extern const char* REN_SET_MATCHCASE		= "Groß/Kleinschreibung beachten";
extern const char* REN_SET_SEARCHPATTERN	= "Suchmuster:";
extern const char* REN_SET_REPLACESTRING	= "Ersetzen durch:";

extern const char* REN_SET_EXT_REPLACE_OLD	= "Alte Endung ersetzen";
extern const char* REN_SET_EXT_UPPERLOWER	= "Groß/Kleinschreibung:";
extern const char* REN_SET_DEFAULT			= "Vorgabe";
extern const char* REN_SET_LOWERCASE		= "kleinbuchstaben";
extern const char* REN_SET_UPPERCASE		= "GROSSBUCHSTABEN";

extern const char* REN_SET_FORMAT			= "Format:";
extern const char* REN_SET_STARTWITH		= "Starte mit:";
extern const char* REN_SET_TEXTBEFORE		= "Text davor:";
extern const char* REN_SET_TEXTBEHIND		= "Text dahinter:";

extern const char* REN_SET_INSERTREPLACE	= "Einfügen oder überschreiben:";
extern const char* REN_SET_INSERT			= "Einfügen";
extern const char* REN_SET_REPLACE			= "Überschreiben mit";
extern const char* REN_SET_TEXT				= "Text:";
extern const char* REN_SET_ATPOSITION		= "An Position:";
extern const char* REN_SET_FROMLEFT			= "von vorne (links)";
extern const char* REN_SET_FROMRIGHT		= "von hinten (rechts)";

extern const char* REN_SET_CONVERTTO		= "Ändern in";

extern const char* REN_SET_FROMPOS			= "Buchstaben entfernen ab Position";
extern const char* REN_SET_TOPOS			= "bis Position";

#endif /* LOCALE_DEUTSCH */
