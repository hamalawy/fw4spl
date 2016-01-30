The OSR scheme (also called OSR) is used by the FW4SPL team for specifying the application build with FW4SPL framework.
OSR  means **Object Service Registry**. The goal of this scheme is to have a global view of the application in terms of Object/Service.

A OSR scheme represents a state of the application at starting (It is expressed by the config.xml file). To recall, the config.xml file describes the object and services used by the application at launch time. It is important to
understand that during the life of the application, some services or objects can be cretaed or destroyed, so sometimes it could be interesting in to do several OSR schemes for the same application at differents moments.

A OSR can be draw at different levels of comprehension:
  * At high level, only the type of the object and the functionality of the services is given. The goal here is to identify the needs (See example below).
  * At technical level, the framework name of the services and objects are introduced (See [Technical OSR scheme](OSR_Scheme#Service_and_object_description_in_the_framework_elements.md)).

Usually, OSR is split in two parts. On the left side, we find the list of the Object used in the application and on the right side the list of the services used. As we know a service is always attached to an object, this is
represented in the scheme by putting objects and associated services on the same line. By line, we neam space between two dashes lines (it can have a lot of services for a object so a simple line isn't enough).

To describe some convention used, we are going to use a **high level** example :

![http://wiki.fw4spl.googlecode.com/hg/OSR_Scheme_Sample.png](http://wiki.fw4spl.googlecode.com/hg/OSR_Scheme_Sample.png)

The **blue arrow** is used to show a link between two services. This link can be named (for example you can have some names like manage, has, start, ...). For example, the OpenFileAction starts the reader file.

When a link showes an **"membership link"**, we use the rectangular shape as we can find in UML notation. The filled rectangle means here composition. In the previous sample, the image visualization is composed by two views.

OSR scheme notation is opened, so users can add his own notation, he just has to introduce some explanaitions on the scheme.

## Service and object description with the framework elements ##

Once the application is described in high level terms, you can transcript your scheme with FW4SPL Object Service names (**Technical level**) like below:

![http://wiki.fw4spl.googlecode.com/hg/OSR_Scheme_TechnicalSample.png](http://wiki.fw4spl.googlecode.com/hg/OSR_Scheme_TechnicalSample.png)

These representations are based on the xml description of each service or object.

Here a sample of a service and its representation in a OSR Scheme

```
   <service implementation="::vtkSimpleNegato::RendererService" type="::fwRender::IRender" uid="myRenderingTuto" autoComChannel="yes" />
```

![http://wiki.fw4spl.googlecode.com/hg/OSR_TechnicalObjectRepresentation.png](http://wiki.fw4spl.googlecode.com/hg/OSR_TechnicalObjectRepresentation.png)

We choose to give the short name for the implementation and the type but it is possible to give the long name. But for big application, the scheme could be a little bit to big.

In the **others attributs** section, we can add all the services attributs which are relevant.

Here a sample of an object and its representation in a OSR Scheme.

```
   <object type="::fwData::Image">
      ....
   </object>
```

![http://wiki.fw4spl.googlecode.com/hg/OSR_TechnicalServiceDescription.png](http://wiki.fw4spl.googlecode.com/hg/OSR_TechnicalServiceDescription.png)

## Interest of the OSR scheme ##

  * Create a OSR scheme for your application allows you to think of your need in terms of objects and services without thinking of the way to write it with FW4SPL.
  * Give a good base for a meeting or team's exchange.

## Application use for scheme drawing ##

> Now, we use Dia (http://projects.gnome.org/dia/) or yEd (http://www.yworks.com/en/products_yed_about.html) but any others could be fine.



