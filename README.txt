== @ENTITY_TOOLBOX ==

SUMMARY
=======
Entity toolbox provides automation for entity related code and functions.
Declare your entity info through hook_entity_toolbox_entity_info() once rather that in multiple hooks.
Entity toolbox will build the entity api hooks info, as well as the entity forms.

Entity toolbox implements the notion of parent/child relation between different entities.
It also provides with useful functions to work with entities.
=======

REQUIREMENTS
============
This module has the following dependencies :
 - ctools
 - entity api
============

FILES
=====
In order to improve readability, the functions were split into several files :
 - entity_toolbox.entities.inc :
   This file contains the entity hooks and controllers related functions.
   In this file, the use of hook_entity_insert() & hook_entity_update() allows to validate their data.
 - entity_toolbox.extend.inc :
   This file contains functions that mean to extend and simplify the use of entity api.
 - entity_toolbox.format.inc :
   This file contains functions that will format data for a various number of hooks and functions.
   Eg : entity_type2entity_label_plural() will transform "foo_category" into "Foo Categories".
 - entity_toolbox.forms.inc :
 - entity_toolbox.functions.inc :
 - entity_toolbox.toolbox.inc :
 - entity_toolbox.tree.inc :
 - entity_toolbox.views.inc :

You can include a file by calling the function entity_toolbox_include().
=====

USING ENTITY TOOLBOX
====================

1. DECLARING AN ENTITY THROUGH ENTITY_TOOLBOX
---------------------------------------------
Declare your entity through hook_entity_toolbox_entity_info(). (@see hook_entity_toolbox_entity_info).
Rather than re declaring this information in the usual entity hooks (hook_entity_info(), hook_entity_info_alter(), etc...), simply call the following functions.

entity_toolbox_entity_info_build() in hook_entity_info().
entity_toolbox_entity_info_alter_build() in hook_entity_info_alter().
entity_toolbox_entity_properties_info_build in hook_entity_properties_info().
entity_toolbox_entity_properties_info_alter_build in hook_entity_properties_info_alter().
entity_toolbox_entity_schema_build() in hook_schema().
entity_toolbox_entity_permissions_build() in hook_permissions().
entity_toolbox_entity_views_data_build() in hook_views_data_alter().
---------------------------------------------

2. ADDING, REMOVING, UPDATING ENTITY PROPERTIES
-----------------------------------------------
Since the information declared in hook_entity_toolbox_entity_info() is used to build the schema version 0 of the entity, you should avoid adding or removing a property in the same hook once the module has been installed.
Instead, use the hook_entity_toolbox_ENTITY_TYPE_properties_update_N() where ENTITY_TYPE is the entity you want to update, and N is the update number.
-----------------------------------------------

3. THE NOTION OF ENTITIES GROUP
-------------------------------
Regrouping entities is a way to classify and easily apply common parameters to a number of entities.
-------------------------------

3. USING THE PARENT CHILD RELATION IN ENTITY TOOLBOX
----------------------------------------------------
A child entity will be able to inherit properties from its parent entity(ies).
----------------------------------------------------

4. EXPOSING AN ENTITY TO VIEWS AND FORMS.
----------------------------------------------------
----------------------------------------------------
====================

TODO : manage properties better
TODO : add entity_toolbox_generator
TODO : add entity_toolbox_complex

GIT REPO
========
http://github.com/krypt0nboy/drupal-entity_toolbox
========
