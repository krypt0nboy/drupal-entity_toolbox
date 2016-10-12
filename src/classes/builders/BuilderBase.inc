<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/HelperBase.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/builders/BuildersHelperBase.inc';

/**
 * Class BuilderBase
 *
 * Base class to build data required by drupal available api's.
 */
class BuilderBase {

  use EntityToolboxCommonTraits, TemplatingTraits {
  }

  /**
   * @var string $modelClass
   *   The builder's model class.
   */
  protected $modelClass;
  /**
   * @var ModelBase $model
   *   The data model object.
   */
  protected $model;
  /**
   * @var array $structure
   *   The structure of the build.
   */
  protected $structure = array();
  /**
   * @var KeyBuilder[] $keyBuilders
   *   The keys builder.
   */
  protected $keyBuilders = array();
  /**
   * @var array $keyBuildersClasses
   *   An array of key builder classes.
   */
  protected $keyBuildersClasses = array();
  /**
   * @var array $keyBuildersArgs
   *   An array containing the key builders args.
   *   Key Builders will be instanciated using those args.
   *   - KEY :
   *    - class :
   */
  protected $keyBuildersArgs = array();
  /**
   * @var EntityToolboxHelperBase[] $helpers
   */
  protected $helpers = array();
  /**
   * @var BuildersHelperBase $helper
   *   The builder's helper.
   */
  protected $helper;
  /**
   * @var integer $phase
   *   The building phase :
   *   - PHZ__INIT : Initializing the builder.
   *   - PHZ__PREP : Preparing the builder.
   *   - PHZ__BUILD : Building.
   *   - PHZ__REBUILD : Rebuilding.
   *   - PHZ__VED : Validating the build/rebuild.
   *   - PHZ__DELIVER : Delivering.
   */
  protected $phase;
  /**
   * @const PHZ__INIT
   */
  const PHZ__INIT = 0x001;
  /**
   * @const PHZ__PREP
   */
  const PHZ__PREP = 0x002;
  /**
   * @const PHZ__BUILD
   */
  const PHZ__BUILD = 0x003;
  /**
   * @const PHZ__REBUILD
   */
  const PHZ__REBUILD = 0x004;
  /**
   * @const PHZ__VED
   */
  const PHZ__VED = 0x005;
  /**
   * @const PHZ__DELIVER
   */
  const PHZ__DELIVER = 0x006;
  /**
   * @var bool $upStructureBeforeBuild
   *   Indicates if the builder structure needs to be updated before building.
   */
  protected $upStructureBeforeBuild;
  /**
   * @var boolean $ready2Prepare
   *   Is the builder ready to prepare.
   */
  protected $ready2Prepare;
  /**
   * @var boolean $ready2GenerateKeyBuilders
   *   Is the builder ready to generate KeyBuilders.
   */
  protected $ready2GenerateKeyBuilders;
  /**
   * @var boolean $ready2Build
   *   Is the builder ready to build.
   */
  protected $ready2Build;
  /**
   * @var bool $ready2Rebuild
   *   Is the builder ready to rebuild.
   */
  protected $ready2Rebuild;
  /**
   * @var boolean $ready2Deliver
   *   Is the builder ready to deliver.
   */
  protected $ready2Deliver;
  /**
   * @var bool $initialized
   */
  protected $initialized;
  /**
   * @var bool $prepared
   */
  protected $prepared;
  /**
   * @var bool $built
   */
  protected $built;
  /**
   * @var bool $validated
   */
  protected $validated;
  /**
   * @var bool $rebuilt
   */
  protected $rebuilt;
  /**
   * @var bool $delivered
   */
  protected $delivered;
  /**
   * @var array $keysRequireRebuild
   */
  protected $keysRequireRebuild = array();
  /**
   * @var array $build
   */
  protected $build = array();

  /*** INIT ***/

  /**
   * BuilderBase constructor.
   *
   * @param array $args
   *   The properties to set.
   *
   * @throws EntityToolboxException
   */
  public function __construct(array $args = array()) {
    $this->setUp($args);
  }

  /**
   * Set up function.
   * Sets the object, and ensures he is ready to build.
   *
   * @param array $args
   *   The args to provide for the builder.
   */
  protected function setUp($args = array()) {
    $this->setPhase(self::PHZ__INIT);
    $this->constructSet($args);
    $this->autoSet();
    $this->prepare();
  }

  /**
   * Init phase where construct args are set.
   *
   * @param array $args
   */
  protected function constructSet($args) {
    $this->setPhase(self::PHZ__INIT);
    $this->setConstructArgs($args);
    $this->bulkSet($args);
  }

  /**
   * Auto sets properties.
   */
  protected function autoSet() {
    $this->generateHelpers();
  }

  /*** INIT END ***/

  /*** SETTERS AND GETTERS ***/

  /**
   * Returns the Model class.
   *
   * @return string
   */
  public function getModelClass() {
    return $this->modelClass;
  }

  /**
   * Sets the Model class.
   *
   * @param string $modelClass
   */
  protected function setModelClass($modelClass) {
    $this->modelClass = $modelClass;
  }

  /**
   * @return ModelBase
   */
  public function getModel() {
    return $this->model;
  }

  /**
   * @param ModelBase $model
   */
  protected function setModel($model) {
    $this->model = $model;
  }

  /**
   * @param $modelClass
   * @param $args
   *
   * @return ModelBase
   */
  protected function modelGetInstance($modelClass, array $args = array()) {
    return new $modelClass($args);
  }

  /**
   * Gets the data structure.
   *
   * @param string $key
   *   A key.
   *
   * @return array
   */
  public function getStructure($key = NULL) {
    return isset($key) ? $this->getKeyParams($key) : $this->structure;
  }

  /**
   * Returns a key structure parameters.
   *
   * @param string $key
   *   The key's name.
   *
   * @return string
   */
  public function getKeyParams($key) {
    $hlp = $this->hlpr();

    return $hlp->keyGet($key);
  }

  /**
   * @param array $structure
   */
  protected function setStructure(array $structure = array()) {
    $this->structure = $structure;
  }

  /**
   * Returns the build.
   *
   * @param null|string $key
   *   A key name.
   *
   * @return array
   */
  public function getBuild($key = NULL) {
    return isset($key) ? $this->getKeyBuild($key) : $this->build;
  }

  /**
   * Returns the build for a given key.
   *
   * @param string $key
   *   The key name.
   *
   * @return mixed
   */
  public function getKeyBuild($key) {
    $hlp = $this->hlpr();
    if ($hlp->keyHasBuild($key)) {
      return $hlp->keyGetBuild($key);
    }
  }

  /**
   * Sets the build.
   *
   * @param array $build
   *   The build to set.
   */
  public function setBuild($build) {
    $this->build = $build;
  }

  /**
   * Sets a key build value.
   *
   * @param string $key
   *   The key name.
   * @param mixed  $build
   *   The build to set.
   *
   * @return mixed
   */
  protected function setKeyBuild($key, $build) {
    $ved               = $this->validateKeyBuild($key, $build);
    $this->build[$key] = $ved;
    $this->postKeyBuild($key);

    return $this->build[$key];
  }

  /**
   * Returns the key current phase.
   *
   * @param string $key
   *   The key name.
   *
   * @return int
   */
  public function keyGetPhase($key) {
    $hlp = $this->hlpr();
    if ($hlp->keyHasBuilder($key)) {
      $builder = $hlp->keyGetBuilder($key);

      return $builder->getPhase();
    }
    else {
      return $this->structure[$key]['phase'];
    }
  }

  /**
   * Sets the build phase for a key.
   *
   * @param string $key
   *   The key's name.
   * @param int    $phase
   *   The phase to set.
   */
  public function keySetPhase($key, $phase) {
    $hlp = $this->hlpr();
    if ($hlp->keyHasBuilder($key)) {
      $builder = $hlp->keyGetBuilder($key);
      $builder->setPhase($phase);
    }
    else {
      $this->structure[$key]['phase'] = $phase;
    }
  }

  /**
   * @return array
   */
  public function getKeyBuildersArgs() {
    return $this->keyBuildersArgs;
  }

  /**
   * @param array $keyBuildersArgs
   */
  public function setKeyBuildersArgs($keyBuildersArgs) {
    $this->keyBuildersArgs = $keyBuildersArgs;
  }

  /**
   * @return BuilderBase[]
   */
  public function getKeyBuilders() {
    return $this->keyBuilders;
  }

  /**
   * Returns a key builder.
   *
   * @param string $key
   *   The key's name.
   *
   * @return \KeyBuilder
   */
  public function getKeyBuilder($key) {
    $hlp = $this->hlpr();
    if (($kBuilder = $hlp->keyGetBuilder($key)) !== NULL) {
      return $kBuilder;
    }
  }

  /**
   * @param BuilderBase[] $keyBuilders
   */
  protected function setKeyBuilders($keyBuilders) {
    foreach ($keyBuilders as $key => $keyBuilder) {
      $this->setKeyBuilder($key, $keyBuilder);
    }
  }

  /**
   * Sets the builder for a given key.
   *
   * @param string            $key
   *   The key name.
   * @param \BuilderBase|NULL $builder
   *   The builder.
   */
  protected function setKeyBuilder($key, BuilderBase $builder = NULL) {
    $this->keyBuilders[$key] = $builder;
  }

  /**
   * @return array
   */
  public function getKeyBuildersClasses() {
    return $this->keyBuildersClasses;
  }

  /**
   * @param array $keyBuildersClasses
   */
  protected function setKeyBuildersClasses($keyBuildersClasses) {
    $this->keyBuildersClasses = $keyBuildersClasses;
  }

  /**
   * @return EntityToolboxHelperBase[]
   */
  public function getHelpers() {
    return $this->helpers;
  }

  /**
   * @param EntityToolboxHelperBase[] $helpers
   */
  public function setHelpers($helpers) {
    $this->helpers = $helpers;
  }

  /**
   * @return BuildersHelperBase
   */
  public function getHelper() {
    return $this->helper;
  }

  /**
   * getHelper alias.
   *
   * @return \BuildersHelperBase
   */
  public function hlpr() {
    return $this->helper;
  }

  /**
   * @param BuildersHelperBase $helper
   */
  public function setHelper($helper) {
    $this->helper = $helper;
  }

  /**
   * Returns the builds current phase.
   *
   * @return int
   */
  public function getPhase() {
    return $this->phase;
  }

  /**
   * Sets the builder current phase.
   *
   * @param int $phase
   *   The phase to set.
   */
  protected function setPhase($phase) {
    $this->phase = $phase;
  }

  /**
   * @return boolean
   */
  public function isReady2Prepare() {
    return $this->ready2Prepare;
  }

  /**
   * @param boolean $ready2Prepare
   */
  public function setReady2Prepare($ready2Prepare) {
    $this->ready2Prepare = $ready2Prepare;
  }

  /**
   * @return boolean
   */
  public function isReady2GenerateKeyBuilders() {
    return $this->ready2GenerateKeyBuilders;
  }

  /**
   * @param boolean $ready2GenerateKeyBuilders
   */
  public function setReady2GenerateKeyBuilders($ready2GenerateKeyBuilders) {
    $this->ready2GenerateKeyBuilders = $ready2GenerateKeyBuilders;
  }

  /**
   * @return boolean
   */
  public function isReady2Build() {
    return $this->ready2Build;
  }

  /**
   * @param boolean $ready2Build
   */
  public function setReady2Build($ready2Build) {
    $this->ready2Build = $ready2Build;
  }

  /**
   * @return boolean
   */
  public function isReady2Rebuild() {
    return $this->ready2Rebuild;
  }

  /**
   * @param boolean $ready2Rebuild
   */
  public function setReady2Rebuild($ready2Rebuild) {
    $this->ready2Rebuild = $ready2Rebuild;
  }

  /**
   * @return boolean
   */
  public function isReady2Deliver() {
    return $this->ready2Deliver;
  }

  /**
   * @param boolean $ready2Deliver
   */
  public function setReady2Deliver($ready2Deliver) {
    $this->ready2Deliver = $ready2Deliver;
  }

  /**
   * @return boolean
   */
  public function isInitialized() {
    return $this->initialized;
  }

  /**
   * @param boolean $initialized
   */
  public function setInitialized($initialized) {
    $this->initialized = $initialized;
  }

  /**
   * @return boolean
   */
  public function isPrepared() {
    return $this->prepared;
  }

  /**
   * @param boolean $prepared
   */
  public function setPrepared($prepared) {
    $this->prepared = $prepared;
  }

  /**
   * @return boolean
   */
  public function isBuilt() {
    return $this->built;
  }

  /**
   * @param boolean $built
   */
  public function setBuilt($built) {
    $this->built = $built;
  }

  /**
   * @return boolean
   */
  public function isValidated() {
    return $this->validated;
  }

  /**
   * @param boolean $validated
   */
  public function setValidated($validated) {
    $this->validated = $validated;
  }

  /**
   * @return boolean
   */
  public function isRebuilt() {
    return $this->rebuilt;
  }

  /**
   * @param boolean $rebuilt
   */
  public function setRebuilt($rebuilt) {
    $this->rebuilt = $rebuilt;
  }

  /**
   * @return boolean
   */
  public function isDelivered() {
    return $this->delivered;
  }

  /**
   * @param boolean $delivered
   */
  public function setDelivered($delivered) {
    $this->delivered = $delivered;
  }

  /**
   * @return boolean
   */
  public function getUpStructureBeforeBuild() {
    return $this->upStructureBeforeBuild;
  }

  /**
   * @param boolean $upStructureBeforeBuild
   */
  public function setUpStructureBeforeBuild($upStructureBeforeBuild) {
    $this->upStructureBeforeBuild = $upStructureBeforeBuild;
  }

  /**
   * @return array
   */
  public function getKeysRequireRebuild() {
    return $this->keysRequireRebuild;
  }

  /**
   * @param array $keysRequireRebuild
   */
  public function setKeysRequireRebuild($keysRequireRebuild) {
    $this->keysRequireRebuild = $keysRequireRebuild;
  }

  /**
   * @param $key
   */
  public function addKeyRequiresRebuild($key) {
    $this->keysRequireRebuild[] = $key;
  }

  /**
   * @param $key
   */
  public function remKeyRequiresRebuild($key) {
    unset($this->keysRequireRebuild[$key]);
  }

  /*** SETTERS AND GETTERS END ***/

  /*** EVAL METHODS ***/

  /**
   * Indicates if the builder requires its structure to be updated before building.
   *
   * @return bool
   */
  protected function requiresStructureUp() {
    $eval = $this->__isset('upStructureBeforeBuild') ? $this->getUpStructureBeforeBuild() : FALSE;

    return $eval;
  }

  /*** EVAL METHODS ENDS ***/

  /**
   * Returns an instance of a helper.
   *
   * @param string $helperClass
   * @param array  $args
   *
   * @return HelperBase
   */
  protected function helperGetInstance($helperClass, array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'data_holder' => $this,
    ), $args);

    return new $helperClass($args);
  }

  /**
   * Returns an instance of a KeyBuilder.
   *
   * @param string $keyBuilderClass
   *   The key builder class.
   * @param array  $args
   *   The args to pass to the key builder instance.
   *
   * @return mixed
   */
  protected function keyBuilderGetInstance($keyBuilderClass, array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'parent_builder' => $this,
    ), $args);

    return new $keyBuilderClass($args);
  }

  /**
   * Returns the builders key that require a rebuild.
   *
   * @return KeyBuilder[]
   */
  protected function keyBuildersRequireRebuild() {
    $require = array();
    foreach ($this->keyBuilders as $key => $keyBuilder) {
      if ($keyBuilder->getRequiresRebuild()) {
        $require[] = $keyBuilder;
      }
    }

    return $require;
  }

  /*** PREPARE ***/

  /**
   * Prepares the builder structure and components.
   */
  protected function prepare() {
    $this->setPhase(self::PHZ__PREP);
    $this->initTemplateVars();
    $this->generateModel();
    $this->prepareStructure();
    $this->generateComponents();
    $this->alterStructure();
  }

  /**
   * Prepares the structure.
   */
  protected function prepareStructure() {
    $method    = $this->__isset('model') ? 'genStructureFromModel' : 'genDynamicStructure';
    $structure = $this->$method();
    $structure = $this->vedStructure($structure);
    $this->__set('structure', $structure);
  }

  /**
   * Validates the structure before it's set.
   *
   * @param array $structure
   *   The prepared structure.
   *
   * @return array
   */
  protected function vedStructure($structure = array()) {
    foreach ($structure as $key => $params) {
      $structure[$key] += array(
        'rebuild' => FALSE,
        'phase'   => self::PHZ__INIT,
      );
    }

    return $structure;
  }

  /**
   * Generates the structure from the model's structure.
   *
   * @return array
   */
  protected function genStructureFromModel() {
    $structure = $this->getModel()->getStructure();

    return $structure;
  }

  /**
   * Generates and set a dynamic structure.
   * Child class that implements this method should return an array.
   *
   * @return array
   */
  protected function genDynamicStructure() {
    return array();
  }

  /**
   * Re generates and set a dynamic structure.
   *
   * @return array
   */
  protected function reGenDynamicStructure() {
    return $this->genDynamicStructure();
  }

  /**
   * Updates structure before building.
   */
  protected function upStructure() {
    $structure = $this->reGenDynamicStructure();
    if (!empty($structure)) {
      $this->__set('structure', $structure);
    }
  }

  /**
   * Alters the structure.
   */
  protected function alterStructure() {
  }

  /**
   * Initialize the template variables.
   *
   * @param array $vars
   *   The variables to init.
   *
   * @return array
   */
  protected function initTemplateVars(array $vars = array()) {
    $this->setTemplateVars($vars);
  }

  /**
   * Instanciates the required components.
   */
  protected function generateComponents() {
    $this->generateKeyBuilders();
  }

  /**
   * Generate and adds required helpers.
   */
  protected function generateHelpers() {
    $this->setHelper($this->mainHelperGetInstance());
  }

  /**
   * @return \BuildersHelperBase
   */
  protected function mainHelperGetInstance() {
    return $this->helperGetInstance('BuildersHelperBase');
  }

  /**
   * Get KeyBuilder instances for each structure key.
   */
  protected function generateKeyBuilders() {
    foreach ($this->keyBuildersClasses as $key => $class) {
      $args = !empty($this->keyBuildersArgs[$key]) ? $this->keyBuildersArgs[$key] : array();
      $args += array(
        'self_key' => $key,
      );
      $this->setKeyBuilder($key, $this->keyBuilderGetInstance($class, $args));
    }
  }

  /**
   * Instanciates and sets a Model instance.
   */
  protected function generateModel() {
    if (isset($this->modelClass)) {
      $this->__set('model', $this->modelGetInstance($this->modelClass));
    }
  }

  /**
   * Called before build.
   */
  protected function preBuild() {
    $hlp = $this->hlpr();
    if ($hlp->ifBuilderRequiresStructureUpdate()) {
      $this->upStructure();
    }
  }

  /**
   * Notifies that a key requires to be rebuilt before delivered.
   *
   * @param string $key
   *   The key to be rebuilt.
   */
  protected function keyRequiresRebuild($key) {
    $hlp = $this->hlpr();
    if ($hlp->builderHasKey($key)) {
      if ($hlp->keyHasBuilder($key)) {
        /** @var KeyBuilder $keyBuilder */
        $keyBuilder = $this->getKeyBuilder($key);
        $keyBuilder->requireRebuild();
      }
      else {
        $this->structure[$key]['rebuild'] = TRUE;
      }
    }
  }

  /**
   * Evaluates if a key requires to be rebuilt.
   *
   * @param string $key
   *   The key's name.
   *
   * @return bool
   */
  public function ifKeyRequiresRebuild($key) {
    return $this->hlpr()->ifKeyRequiresRebuild($key);
  }

  /*** PREPARE END ***/

  /**
   * Return the built/rebuilt data.
   *
   * @return mixed
   */
  public function build() {
    $phase = $this->getPhase() == self::PHZ__BUILD ? self::PHZ__REBUILD : self::PHZ__BUILD;
    $this->setPhase($phase);
    $this->preBuild();
    $this->buildDefault();
    $this->validateBuild();
    $this->postBuild();

    return $this->deliver();
  }

  /**
   * Return the rebuilt data.
   *
   * @return mixed
   */
  public function rebuild() {
    return $this->build();
  }

  /**
   * Default build function.
   */
  protected function buildDefault() {
    foreach (array_keys($this->structure) as $key) {
      $this->buildKey($key);
    }
  }

  /**
   * Builds the value for a given key.
   *
   * @param string $key
   *   The key to build.
   *
   * @return mixed
   */
  protected function buildKey($key) {
    $hlp            = $this->hlpr();
    $hasBuildMethod = $hlp->keyHasBuildMethod($key);
    $hasDefaultVal  = $hlp->keyHasDefaultValue($key);
    if ($hasBuildMethod) {
      $build = $this->keyBuildMethod($key);
      if (is_null($build) && $hasDefaultVal) {
        return $this->buildKeyDefault($key);
      }
      else {
        return $this->setKeyBuild($key, $build);
      }
    }
    else {
      if (isset($hasDefaultVal)) {
        return $this->buildKeyDefault($key);
      }
    }
  }

  /**
   * Calls the key build method.
   *
   * @param string $key
   *   The key name.
   * @param bool   $call
   *   Whether to call the method or not.
   *
   * @return mixed
   */
  protected function keyBuildMethod($key, $call = TRUE) {
    $hlp    = $this->hlpr();
    $method = $hlp->keyGetBuildMethod($key);

    return $call ? $this->$method() : $method;
  }

  /**
   * Calls the key validation method.
   *
   * @param string $key
   *   The key name.
   * @param mixed  $toVed
   *   The build to validate.
   * @param bool   $call
   *   Whether to call the method or not.
   *
   * @return mixed
   */
  protected function keyVedMethod($key, $toVed, $call = TRUE) {
    $hlp = $this->hlpr();
    if ($hlp->keyHasVedMethod($key)) {
      $method = $hlp->keyGetVedMethod($key);

      return $call ? $this->$method($toVed) : $method;
    }
    else {
      return $toVed;
    }
  }

  /**
   * Calls custom post key build method.
   *
   * @param string $key
   *   The key name.
   * @param bool   $call
   *   Whether to call the method or not.
   *
   * @return void|mixed
   */
  protected function keyPostBuildMethod($key, $call = TRUE) {
    $hlp = $this->hlpr();
    if ($hlp->keyHasPostBuildMethod($key)) {
      $method = $hlp->keyGetPostBuildMethod($key);
      if ($call) {
        $this->$method();
      }
      else {
        return $method;
      }
    }
  }

  /**
   * Calls custom key static deliver fail fix method.
   *
   * @param string $key
   *   The key name.
   * @param bool   $call
   *   Whether to call the method or not.
   *
   * @return void|mixed
   */
  protected function keyStaticDeliverFailFixMethod($key, $call = TRUE) {
    $hlp = $this->hlpr();
    if (($method = $hlp->keyGetStaticDeliverFailFixMethod($key)) !== NULL) {
      if ($call) {
        $this->$method();
      }
      else {
        return $method;
      }
    }
  }

  /**
   * Rebuilds a key value.
   *
   * @param string $key
   *   The key name.
   *
   * @return mixed
   */
  protected function rebuildKey($key) {
    $this->buildKey($key);
  }

  /**
   * Default build method for a given key.
   *
   * @param string $key
   *   The key name.
   *
   * @return mixed
   */
  protected function buildKeyDefault($key) {
    $hlp     = $this->hlpr();
    $default = $hlp->keyGetDefault($key);
    $build   = $this->processValueTemplate($default);

    return $this->setKeyBuild($key, $build);
  }

  /**
   * Called after the key data has been built.
   *
   * @param string $key
   *   The key name.
   */
  protected function postKeyBuild($key) {
    $this->updateKeyBuildTemplateVars($key);
    $this->keyPostBuildMethod($key);
  }

  /**
   * Updates the template vars from a key build.
   *
   * @param string $key
   * @param null   $replaceMarker
   */
  protected function updateKeyBuildTemplateVars($key, $replaceMarker = NULL) {
    $replaceMarker = isset($replaceMarker) ? $replaceMarker : '';
    $extract       = array($key => $this->build[$key]);
    $flattened     = $this->flattenExtract('', $replaceMarker, $extract);
    $this->updateTemplateVars($flattened);
  }

  /**
   * Method called after the build has been set,
   * and before it is delivered.
   */
  protected function postBuild() {
  }

  /**
   * Method called after the rebuild has been set,
   * and before it is delivered.
   */
  protected function postReBuild() {
  }

  /**
   * Returns the build.
   *
   * @return array
   */
  protected function deliver() {
//    $hlp = $this->hlpr();
//    if ($hlp->ifBuilderIsReady2Deliver()) {
//      return $this->retrieveBuild();
//    }
//    else {
//      $this->deliverFailFix();
//
//    }
    return $this->retrieveBuild();
  }

  /**
   * Returns the build, retrieved from key builders.
   *
   * @return mixed
   */
  protected function retrieveBuild() {
    return $this->build;
  }

  /**
   * Calls fix methods when deliverEval has failed.
   */
  public function deliverFailFix() {
    $hlp   = $this->hlpr();
    $toFix = $hlp->builderGetKeysDeliverFail();
//    $this->deliverFailFixExec();
//    /**
//     * @var string $key
//     *   The key that failed to deliver.
//     */
//    foreach ($toFix as $key) {
//      if (($keyBuilder = $hlp->keyGetBuilder($key)) !== NULL) {
//        $keyBuilder->deliverFailFix();
//      }
//      else {
//        $this->staticDeliverFailFix($key);
//      }
//    }
  }

  /**
   * Method that allows to deliver a key build when failed.
   *
   * @param string $key
   *   The key's name.
   */
  protected function staticDeliverFailFix($key) {
    $method = $this->keyStaticDeliverFailFixMethod($key, FALSE);
    if (isset($method)) {
      $this->$method();
      $this->deliveryFailIsFixed($key);
    }
    else {
      $this->rebuildKey($key);
      $this->deliveryFailIsFixed($key);
    }
  }

  /**
   * Deliver fail fix method.
   * Any child class should have its own method.
   */
  protected function deliverFailFixExec() {
  }

  /**
   * Sets as fixed the delivery for a given key.
   *
   * @param string $key
   *   The key.
   */
  protected function deliveryFailIsFixed($key) {
    $hlp = $this->hlpr();
    if (($keyBuilder = $hlp->keyGetBuilder($key)) !== NULL) {
      $this->setReady2Deliver(TRUE);
    }
    else {
      $this->structure[$key]['rebuild'] = FALSE;
    }
  }

  /**
   * @param $value
   * @return string
   */
  protected function processValueTemplate($value) {
    return $this->isValueTemplate($value) ? string_template_process($value, $this->getTemplateVars()) : $value;
  }

  /**
   * Validates the build before it can be delivered.
   */
  protected function validateBuild() {
    $this->unsetKeysEval();
    $this->sortBuild();
  }

  /**
   * Sorts the build keys.
   */
  protected function sortBuild() {
    uasort($this->build, 'drupal_sort_weight');
  }

  /**
   * Validates a key build.
   *
   * @param string $key
   *   The key.
   * @param mixed  $build
   *   The build to validate.
   *
   * @return mixed
   */
  protected function validateKeyBuild($key, $build) {
    $build = $this->validateKeyBuildType($key, $build);
    $build = $this->keyVedMethod($key, $build);

    return $build;
  }

  /**
   * Validates the key build type.
   *
   * @param string $key
   *   The key.
   * @param mixed  $build
   *   The build to validate.
   *
   * @return mixed
   */
  protected function validateKeyBuildType($key, $build) {
    $hlp       = $this->hlpr();
    $type      = $hlp->keyGetType($key, TRUE);
    $buildType = gettype($build);
    $vedMethod = 'validate' . ucfirst($type) . 'Value';
    if ($type == 'mixed') {
      return $this->validateMixedValue($build, $buildType, $hlp->keyGetType($key));
    }
    else {
      if (method_exists($this, $vedMethod)) {
        return $this->$vedMethod($build, $buildType);
      }
    }

    return $build;
  }

  /**
   * @param $valueType
   * @param $matchType
   * @return bool
   */
  protected function matchType($valueType, $matchType) {
    return $valueType == $matchType;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateBooleanValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'boolean');
    if (!$matchType) {
      if ($this->isValueTemplate($value)) {
        $value = FALSE;
      }
      else {
        settype($value, 'boolean');
      }
    }

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateIntegerValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'integer');
    if (!$matchType) {
      if ($this->isValueTemplate($value)) {
        $value = 0;
      }
      else {
        settype($value, 'integer');
      }
    }

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateDoubleValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'double');
    if (!$matchType) {
      if ($this->isValueTemplate($value)) {
        $value = 0;
      }
      else {
        settype($value, 'double');
      }
    }

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateStringValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'string');
    if (!$matchType) {
      settype($value, 'string');
    }

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateArrayValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'array');
    if (!$matchType) {
      $value = array();
    }

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateObjectValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'object');

    return $value;
  }

  /**
   * @param $value
   * @param $valueType
   * @return mixed
   */
  protected function validateNullValue($value, $valueType) {
    $matchType = $this->matchType($valueType, 'null');
    if (!$matchType) {
      settype($value, 'null');
    }

    return $value;
  }

  /**
   * @param      $value
   * @param      $valueType
   * @param null $mixed
   * @return mixed
   */
  protected function validateMixedValue($value, $valueType, $mixed = NULL) {
    if (isset($mixed)) {
      $types = extract_types($mixed);
      if (!in_array($valueType, $types)) {
        if ($this->isValueTemplate($value)) {
          $value = '';
        }
        settype($value, $types[0]);
      }
    }

    return $value;
  }

  /**
   * Evaluates if a key build should be unset from the build.
   */
  protected function unsetKeysEval() {
    $keys = array_flip(array_keys($this->build));
    $mod  = 0;
    foreach (array_keys($this->build) as $key) {
      $unset = $this->unsetKeyEval($key);
      if ($unset) {
        $pos = $keys[$key] - $mod;
        array_splice($this->build, $pos, 1);
        $mod++;
      }
    }
  }

  /**
   * Evaluates if a key should be unset from the build.
   *
   * @param string $key
   *   The key's name.
   *
   * @return bool
   */
  protected function unsetKeyEval($key) {
    $hlp   = $this->hlpr();
    $build = $hlp->keyGetBuild($key);
    $eval  = FALSE;
    if (($unsetIf = $hlp->keyGetUnsetConditions($key)) !== NULL) {
      $negate = preg_match('/^!\w+/', $unsetIf);
      if ($negate) {
        $unsetIf = preg_replace('/^!/', '', $unsetIf);
      }
      switch ($unsetIf) {
        case 'false':
          $eval = $build == FALSE;
          break;
        case 'true':
          $eval = $build == TRUE;
          break;
        default:
          $isCallback = preg_match('/(?P<callback>.+)(:c:)$/', $unsetIf, $matches);
          if ($isCallback) {
            $isCallable = is_callable($matches['callback']);
            $eval       = $isCallable ? $matches['callback']($build) : $build == $unsetIf;
          }
          else {
            $eval = $build == $unsetIf;
          }
          break;
      }

      return $negate ? !$eval : $eval;
    }
    else {
      return $eval;
    }
  }
}