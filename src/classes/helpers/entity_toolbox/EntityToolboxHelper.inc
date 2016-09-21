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


  /*** GETTERS END ***/

  /*** HASSERS ***/
  /*** HASSERS END ***/
}