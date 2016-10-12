<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/HelperBase.inc';

/**
 * Class BuildersHelperBase
 */
class BuildersHelperBase extends HelperBase {
  /** {{@inheritdoc}} */
  const ALIAS = 'structure';

  /**
   * @return BuilderBase
   */
  public function getDataHolder() {
    return parent::getDataHolder();
  }

  /**
   * Swaps to structure property. (reset).
   *
   * @return string
   */
  protected function swap2Structure() {
    return $this->resetAlias();
  }

  /**
   * Swaps to model property.
   *
   * @return string
   */
  protected function swap2Model() {
    return $this->swapAlias('model');
  }

  /**
   * Swaps to keyBuilders property.
   *
   * @return string
   */
  protected function swap2KeyBuilders() {
    return $this->swapAlias('keyBuilders');
  }

  /**
   * Swaps to build property.
   *
   * @return string
   */
  protected function swap2Build() {
    return $this->resetAlias();
  }

  /***  ***/

  /**
   * Returns the builder structure.
   *
   * @param string $name
   *   A structure key.
   *
   * @return array
   */
  public function structGet($name = NULL) {
    $struct = $this->alias('structure');

    return $struct;
  }

  /**
   * Returns the builder model object.
   *
   * @return ModelBase
   */
  public function modelGet() {
    return $this->alias('model');
  }

  /**
   * Returns the method to generate the builder's structure.
   *
   * @return string
   */
  public function modelGenMethodGet() {
    return $this->builderHasModel() ? 'genStructureFromModel' : 'genDynamicStructure';
  }

  /**
   * Returns the keys that failed to deliver.
   *
   * @return string[]
   */
  public function builderGetKeysDeliverFail() {
  }

  /**
   * Returns the builder keyBuilders.
   *
   * @param string $name
   *   A key builder identifier.
   *
   * @return KeyBuilder[]
   */
  public function keyBuildersGet($name = NULL) {
    $kBuilders = $this->alias('keyBuilders');
    if (isset($name)) {
      if (!empty($kBuilders)) {
        if (!empty($kBuilders[$name])) {
          return $kBuilders[$name];
        }
      }
    }
    else {
      return $kBuilders;
    }
  }

  /**
   * Returns the builder build.
   *
   * @param string $name
   *   A key from the build.
   *
   * @return array
   */
  public function buildGet($name = NULL) {
    $build = $this->alias('build');
    if (isset($name)) {
      if (!empty($build)) {
        if (isset($build[$name])) {
          return $build[$name];
        }
      }
    }
    else {
      return $build;
    }
  }

  /**
   * Returns a key params from the builder's structure.
   *
   * @param string $name
   *   The key name.
   *
   * @return array
   */
  public function keyGet($name) {
    $struct = $this->structGet();
    if (isset($struct[$name])) {
      return $struct[$name];
    }
  }

  /**
   * Returns the variable type of a key within the builder's structure.
   *
   * @param string $name
   *   The key name.
   * @param bool   $translate
   *   Whether to translate to simpler type.
   *   Eg : If set TRUE :
   *    - int|bool becomes mixed.
   *
   * @return string
   */
  public function keyGetType($name, $translate = FALSE) {
    $key = $this->keyGet($name);

    return $translate ? translate_type($key['type']) : $key['type'];
  }

  /**
   * Returns the build method of a key within the builder's structure.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyGetBuildMethod($name) {
    $key = $this->keyGet($name);

    if (!empty($key['method'])) {
      return $key['method'];
    }
  }

  /**
   * Returns the build method of a key within the builder's structure.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyGetVedMethod($name) {
    $cKey   = str2camelcase($name);
    $method = 'vedKey' . $cKey;
    if (method_exists($this->getDataHolder(), $method)) {
      return $method;
    }
  }

  /**
   * Returns the post build method of a key.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyGetPostBuildMethod($name) {
    $cKey   = str2camelcase($name);
    $method = 'postBuildKey' . $cKey;
    if (method_exists($this->getDataHolder(), $method)) {
      return $method;
    }
  }

  /**
   * Returns the static deliver fail fix method of a key.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyGetStaticDeliverFailFixMethod($name) {
    $cKey   = str2camelcase($name, FALSE);
    $method = $cKey . 'StaticDeliverFailFix';
    if (method_exists($this->getDataHolder(), $method)) {
      return $method;
    }
  }

  /**
   * Returns the default value of a key within the builder's structure.
   *
   * @param string $name
   *   The key name.
   *
   * @return mixed
   */
  public function keyGetDefault($name) {
    $key  = $this->keyGet($name);
    $type = $this->keyGetType($name);
    switch ($type) {
      case 'boolean':
        if (!empty($key['default'])) {
          if (!is_null($key['default'])) {
            return $key['default'];
          }
        }
        break;
      case 'mixed':
        $types = extract_types($type);
        if (in_array('boolean', $types)) {
          if (!is_null($key['default'])) {
            return $key['default'];
          }
        }
        else {
          return $key['default'];
        }
        break;
      default:
        if (!empty($key['default'])) {
          return $key['default'];
        }
        break;
    }
  }

  /**
   * Returns the unset conditions of a key within the builder's structure.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyGetUnsetConditions($name) {
    $key = $this->keyGet($name);

    if (isset($key['unset if'])) {
      return $key['unset if'];
    }
  }

  /**
   * Returns a key build.
   *
   * @param string $name
   *   The key name.
   *
   * @return mixed
   */
  public function keyGetBuild($name) {
    $build = $this->buildGet();

    if (isset($build[$name])) {
      return $build[$name];
    }
  }

  /**
   * Returns a key build value type.
   *
   * @param string $name
   *   The key name.
   *
   * @return string
   */
  public function keyBuildGetType($name) {
    return gettype($this->keyGetBuild($name));
  }

  /**
   * Returns a key build status.
   *
   * @param string $name
   *   The key name.
   *
   * @return int
   */
  public function keyGetStatus($name) {
  }

  /**
   * Returns the key builder for a given key.
   *
   * @param string $name
   *   The key name.
   *
   * @return KeyBuilder
   */
  public function keyGetBuilder($name) {
    return $this->keyBuildersGet($name);
  }

  /***  ***/

  /***  ***/

  /**
   * Indicates if the builder has a set structure.
   *
   * @return bool
   */
  public function builderHasStructure() {
    return $this->dataHolderHasProperty('structure', TRUE);
  }

  /**
   * Indicates if the builder has a set model.
   *
   * @return bool
   */
  public function builderHasModel() {
    return $this->dataHolderHasProperty('model', TRUE);
  }

  /**
   * Indicates if the builder has key builders.
   *
   * @return bool
   */
  public function builderHasKeyBuilders() {
    return $this->dataHolderHasProperty('key_builders', TRUE);
  }

  /**
   * Indicates if the builder has a build.
   *
   * @return bool
   */
  public function builderHasBuild() {
    return $this->dataHolderHasProperty('build', TRUE);
  }

  /**
   * Indicates if the builder has a given key within it's structure.
   *
   * @param string $name
   *   The key's name.
   *
   * @return bool
   */
  public function builderHasKey($name) {
    return !empty($this->keyGet($name));
  }

  /**
   * Indicates if the builder has key builder for a given key.
   *
   * @param string $name
   *   The key for which to get the key builder.
   *
   * @return bool
   */
  public function keyHasBuilder($name) {
    return !empty($this->keyBuildersGet($name));
  }

  /**
   * Indicates if the builder has a build for a given key.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasBuild($name) {
    return !empty($this->keyGetBuild($name));
  }

  /**
   * Indicates if the a key has a default value.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasDefaultValue($name) {
    return !empty($this->keyGetDefault($name));
  }

  /**
   * Indicates if the a key has a unset conditions.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasUnsetConditions($name) {
    $conditions = $this->keyGetUnsetConditions($name);

    return isset($conditions);
  }

  /**
   * Indicates if the a key has a given method.
   *
   * @param string      $name
   *   The key name.
   * @param null|string $method
   *   The method.
   *
   * @return bool
   */
  public function keyHasMethod($name, $method = NULL) {
    return !empty($this->keyGetBuildMethod($name));
  }

  /**
   * Indicates if the a key has a build method.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasBuildMethod($name) {
    return !empty($this->keyGetBuildMethod($name));
  }

  /**
   * Indicates if the a key has a validation method.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasVedMethod($name) {
    return !empty($this->keyGetVedMethod($name));
  }

  /**
   * Indicates if the a key has a post build method.
   *
   * @param string $name
   *   The key name.
   *
   * @return bool
   */
  public function keyHasPostBuildMethod($name) {
    return !empty($this->keyGetPostBuildMethod($name));
  }

  /***  ***/

  /***  ***/

  /**
   * Indicates if the key is of type array.
   *
   * @param string $name
   *   The key.
   *
   * @return bool
   */
  public function keyTypeIsArray($name) {
    return $this->keyGetType($name) == 'array';
  }

  /***  ***/

  /***  ***/

  /**
   * Evaluates if the builder is ready to generate key builders.
   *
   * @param bool $autoSet
   *   Whether to autoset ready2Build.
   * @param bool $reset
   *   Whether to re evaluate if the ready to build.
   *
   * @return bool
   */
  public function ifBuilderIsReady2GenerateKeyBuilders($autoSet = TRUE, $reset = FALSE) {
  }

  /**
   * Evaluates if the builder is ready to build.
   *
   * @param bool $autoSet
   *   Whether to autoset ready2Build.
   * @param bool $reset
   *   Whether to re evaluate if the ready to build.
   *
   * @return bool
   */
  public function ifBuilderIsReady2Build($autoSet = TRUE, $reset = FALSE) {
  }

  /**
   * Evaluates if the builder is ready to deliver.
   *
   * @param bool $autoSet
   *   Whether to autoset ready2Build.
   * @param bool $reset
   *   Whether to re evaluate if the ready to build.
   *
   * @return bool
   */
  public function ifBuilderIsReady2Deliver($autoSet = TRUE, $reset = FALSE) {
    $evaluated   = $this->getDataHolder()->isReady2Deliver();
    $isEvaluated = isset($evaluated);
    if ($reset || !$isEvaluated) {
      $evals = array();
      foreach ($this->structGet() as $key => $params) {
        if (($keyBuilder = $this->keyGetBuilder($key)) !== NULL) {
          $kHlpr       = $keyBuilder->hlpr();
          $eval        = $kHlpr->ifBuilderIsReady2Deliver($autoSet, $reset);
          $evals[$key] = $eval;
        }
        else {
          $keyRequiresRebuild = $params['rebuild'];
          $eval               = !$keyRequiresRebuild;
          $evals[$key]        = $eval;
        }
      }
      $ready = in_array(FALSE, $evals, TRUE);
      if ($autoSet) {
        $this->getDataHolder()->setReady2Deliver($ready);

        return $ready;
      }
    }
    else {
      return $evaluated;
    }
  }

  /**
   * Evaluates if the builder's structure needs to be updated.
   *
   * @param bool $autoSet
   *   Whether to autoset ready2Build.
   * @param bool $reset
   *   Whether to re evaluate if the ready to build.
   *
   * @return bool
   */
  public function ifBuilderRequiresStructureUpdate($autoSet = TRUE, $reset = FALSE) {
    return $this->getDataHolder()->getUpStructureBeforeBuild();
  }

  /**
   * Evaluates if a key requires to be rebuilt before it is delivered.
   *
   * @param string $name
   *   The key's name.
   *
   * @return bool
   */
  public function ifKeyRequiresRebuild($name) {
    if ($this->keyHasBuilder($name)) {
      $keyBuilder = $this->keyGetBuilder($name);

      return $keyBuilder->getRequiresRebuild();
    }
    else {
      $key = $this->keyGet($name);

      return $key['rebuild'];
    }
  }

  /***  ***/
}