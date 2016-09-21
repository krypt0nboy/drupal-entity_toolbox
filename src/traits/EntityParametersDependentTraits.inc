<?php

/**
 * Class EntityParametersDependentTraits
 */
trait EntityParametersDependentTraits {
  /**
   * @var bool $fieldable
   *   Whether the entity is fieldable or not.
   */
  protected $fieldable;
  /**
   * @var boolean $hasRevisions
   *   Whether the entity has revisions or not.
   */
  protected $hasRevisions;
  /**
   * @var boolean $hasTranslations
   *   Whether the entity has translations or not.
   */
  protected $hasTranslations;
  /**
   * @var bool $exportable
   *   Whether the entity is exportable or not.
   */
  protected $exportable;
  /**
   * @var string $rootPath
   *   The root path.
   */
  protected $rootPath;
  /**
   * @var string[] $bundles
   *   The entity type bundles.
   */
  protected $bundles = array();
  /**
   * @var string $bundleEntity
   *   The bundle entity.
   */
  protected $bundleEntity;
  /**
   * @var string $bundleOf
   *   What entity type is this entity the bundle of.
   */
  protected $bundleOf;

  /**
   * @return boolean
   */
  public function isFieldable() {
    return $this->fieldable;
  }

  /**
   * @param boolean $fieldable
   */
  public function setFieldable($fieldable) {
    $this->fieldable = $fieldable;
  }

  /**
   * @return boolean
   */
  public function hasRevisions() {
    return $this->hasRevisions;
  }

  /**
   * @param boolean $hasRevisions
   */
  public function setHasRevisions($hasRevisions) {
    $this->hasRevisions = $hasRevisions;
  }

  /**
   * @return boolean
   */
  public function isHasTranslations() {
    return $this->hasTranslations;
  }

  /**
   * @param boolean $hasTranslations
   */
  public function setHasTranslations($hasTranslations) {
    $this->hasTranslations = $hasTranslations;
  }

  /**
   * @return boolean
   */
  public function isExportable() {
    return $this->exportable;
  }

  /**
   * @param boolean $exportable
   */
  public function setExportable($exportable) {
    $this->exportable = $exportable;
  }

  /**
   * @return string
   */
  public function getRootPath() {
    return $this->rootPath;
  }

  /**
   * @param string $rootPath
   */
  public function setRootPath($rootPath) {
    $this->rootPath = $rootPath;
  }

  /**
   * @return string[]
   */
  public function getBundles() {
    return $this->bundles;
  }

  /**
   * @param string[] $bundles
   */
  public function setBundles($bundles) {
    $this->bundles = $bundles;
  }

  /**
   * @return string
   */
  public function getBundleEntity() {
    return $this->bundleEntity;
  }

  /**
   * @param string $bundleEntity
   */
  public function setBundleEntity($bundleEntity) {
    $this->bundleEntity = $bundleEntity;
  }

  /**
   * @return string
   */
  public function getBundleOf() {
    return $this->bundleOf;
  }

  /**
   * @param string $bundleOf
   */
  public function setBundleOf($bundleOf) {
    $this->bundleOf = $bundleOf;
  }
}