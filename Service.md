# Service #

## Utilisation d'un services ##

Un utilisateur manipule un **service** gr�ce � plusieurs m�thodes. Il s'agit des m�thodes **configure**, **start**, **stop**, **update(message)** et **update** :

  * La m�thode **configure** permet de configurer les param�tres du **service**. Par exemple, dans le cas d�un **service** de lecture d'images m�dicales, la localisation du fichier � ouvrir doit �tre effectu�e au niveau de cette m�thode. Cette m�thode analyse la configuration du **service**.
  * La m�thode **start** permet de lancer le **service** (attention, cela ne revient pas � instancier le **service**). Par exemple, dans le cas d'un **service** de visualisation d'une image m�dicale, la m�thode **start** associe un canevas au **service**, de mani�re � connaitre la fen�tre dans laquelle les donn�es vont �tre repr�sent�es.
  * La m�thode **stop** permet d'arr�ter le **service**. De surcro�t, il est possible d�utiliser les m�thodes **start** et **stop** plusieurs fois tout en conservant la m�me instance du **service**. Par exemple, dans le cas d�un **service** de lecture d'images m�dicales, la m�thode **stop** d�truit le canevas qui est associ� au **service**.
  * La m�thode **update** est appel�e pour que le **service** effectue une action sur la **data** qui lui est associ�e. Par exemple, dans le cas d�un **service** de lecture d'images m�dicales, le **service** lit l'image m�dicale, la convertit, puis stocke ces nouvelles donn�es dans la **data** qui lui est associ�e.
  * La m�thode **update(message)** est appel�e lorsque le **service** re�oit un message provenant d'une ou des **data** qu'il �coute (dans les fait, ce ne sont �videment pas les **data** qui envoient des messages, mais le **service d'�dition** associ� � la **data**. Par exemple, dans le cas d�un **service** de lecture d'images m�dicales, apr�s avoir lu l'image m�dicale, le **service** envoie un message aux **services** associ�es � l'image via le **service d'�dition**. Le **service** de visualisation d'une image m�dicale re�oit ce message et modifie en cons�quence l'affichage de cette nouvelle image m�dicale.

> ## Comportement d'un service ##


> ![http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png](http://wiki.fw4spl.googlecode.com/hg/SimpleStates.png)

