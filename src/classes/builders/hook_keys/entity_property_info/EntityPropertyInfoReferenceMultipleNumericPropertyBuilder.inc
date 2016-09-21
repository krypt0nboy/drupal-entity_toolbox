<?php

/**
 * Class EntityPropertyInfoReferenceNumericPropertyBuilder
 */
class EntityPropertyInfoReferenceMultipleNumericPropertyBuilder extends EntityPropertyInfoReferenceNumericPropertyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'model_class' => 'EntityPropertyInfoReferenceMultipleNumericPropertyModel',
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function buildType() {
    return 'list<' . parent::buildType() . '>';
  }
}