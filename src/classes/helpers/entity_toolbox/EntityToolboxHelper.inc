<?php

require_once 'EntityToolboxHelperBase.inc';

/**
 * Class EntityToolboxHelper
 */
class EntityToolboxHelper extends EntityToolboxHelperBase {
  /** {{@inheritdoc}} */
  const ALIAS = 'toolboxInfo';

  /*** EXTRACT ***/

  /**
   * @param array $args
   */
  public function dataToArgs(array &$args = array()) {
    $info = $this->alias();
    foreach ($info as $key => $arg) {
      $key        = preg_replace('/\s/', '_', $key);
      $args[$key] = $arg;
    }
  }

  /*** EXTRACT END ***/

  /*** ISSERS ***/

  public function isFieldable() {
    $info = $this->alias();

    return $info['fieldable'];
  }

  public function isExportable() {
  }

  /*** ISSERS END ***/

  /*** GETTERS END ***/

  /**
   * Returns the labels.
   *
   * @return string[]
   */
  public function labels() {
    $data = $this->alias();

    return $data['labels'];
  }

  /**
   * Returns a label for a given label type.
   *
   * @param string $type
   *   The label type.
   *
   * @return string
   */
  public function label($type) {
    $labels = $this->labels();
    if (!empty($labels[$type])) {
      return $labels[$type];
    }
  }

  /**
   * Returns the entity type single label.
   *
   * @return string
   */
  public function labelSingle() {
    return $this->label('single');
  }

  /**
   * Returns the entity type plural label.
   *
   * @return string
   */
  public function labelPlural() {
    return $this->label('plural');
  }

  /**
   * Returns the entity type single label.
   *
   * @return string
   */
  public function labelSingleLowerCase() {
    return $this->label('single lowercase');
  }

  /**
   * Returns the entity type plural label.
   *
   * @return string
   */
  public function labelPluralLowerCase() {
    return $this->label('plural lowercase');
  }

  /**
   * Returns the entity type plural machine_name.
   *
   * @return string
   */
  public function machineNamePlural() {
    return $this->label('machine_name_plural');
  }

  /**
   * Returns the classes.
   *
   * @return array
   */
  public function classes() {
    $data = $this->alias();

    return $data['classes'];
  }

  /**
   * Returns a class.
   *
   * @param string $name
   *   The class to get.
   *
   * @return array
   */
  public function classGet($name) {
    $classes = $this->classes();

    if (!empty($classes[$name])) {
      return $classes[$name];
    }
  }

  public function bundleOf() {
    $info = $this->alias();

    return $info['bundle_of'];
  }

  public function bundleEntity() {
    $info = $this->alias();

    return $info['bundle_entity'];
  }

  /**
   * Returns the entity type root path.
   *
   * @return mixed
   */
  public function rootPath() {
    return $this->alias()['root path'];
  }

  /**
   * Returns the entity type callbacks.
   *
   * @return mixed
   */
  public function callbacksGet() {
    $info = $this->alias();

    return $info['callbacks'];
  }

  /**
   * Returns a specific callback.
   *
   * @param $callback
   */
  public function callbackGet($callback) {
    $cbacks = $this->callbacksGet();

    if (isset($cbacks[$callback])) {
      return $cbacks[$callback];
    }
  }

  /*** GETTERS END ***/

  /*** HASSERS ***/

  public function hasRevisions() {
  }

  public function hasTranslations() {
  }

  /*** HASSERS END ***/
}