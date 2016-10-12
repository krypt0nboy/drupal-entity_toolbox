<?php

/**
 * Class EntityToolboxInfoSelfReferencePropertyBuilder
 */
class EntityToolboxInfoSelfReferencePropertyBuilder extends EntityToolboxInfoReferencePropertyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function constructSet($args) {
    parent::constructSet($args);
  }

  /** {{@inheritdoc}} */
  public function requireRebuild() {
    parent::requireRebuild();
    /** @var EntityToolboxInfoBuilder $hookBuilder */
    $hookBuilder  = $this->getHookBuilder();
    $keys2Rebuild = array(
      'tables',
      'keys',
      'children_inherit',
      'entity_info'
    );
    foreach ($keys2Rebuild as $key) {
      $hookBuilder->keyRequiresRebuild($key);
    }
  }

  /** {{@inheritdoc}} */
  protected function postSetReference() {
    if ($this->getPhase() == self::PHZ__REBUILD) {
      parent::postSetReference();
    }
  }

  /** {{@inheritdoc}} */
  public function setReferenceInfo($referenceInfo) {
    parent::setReferenceInfo($referenceInfo);
  }

  /**
   * Builds "self_numeric_value".
   */
  protected function buildSelfNumericProperty() {
    $idKey = concat($this->entityType, 'id');
    $idKey = concat('parent', $idKey);

    return $idKey;
  }
}