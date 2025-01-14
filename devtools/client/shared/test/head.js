/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

var {require} = Cu.import("resource://gre/modules/devtools/shared/Loader.jsm", {});
var {TargetFactory} = require("devtools/client/framework/target");
var {console} = Cu.import("resource://gre/modules/devtools/shared/Console.jsm", {});
var {gDevTools} = Cu.import("resource:///modules/devtools/client/framework/gDevTools.jsm", {});
const {DOMHelpers} = Cu.import("resource:///modules/devtools/client/shared/DOMHelpers.jsm", {});
const {Hosts} = require("devtools/client/framework/toolbox-hosts");
const {defer} = require("promise");
const DevToolsUtils = require("devtools/shared/DevToolsUtils");

DevToolsUtils.testing = true;
SimpleTest.registerCleanupFunction(() => {
  DevToolsUtils.testing = false;
});

const TEST_URI_ROOT = "http://example.com/browser/devtools/client/shared/test/";
const OPTIONS_VIEW_URL = TEST_URI_ROOT + "doc_options-view.xul";

/**
 * Open a new tab at a URL and call a callback on load
 */
function addTab(aURL, aCallback)
{
  waitForExplicitFinish();

  gBrowser.selectedTab = gBrowser.addTab();
  content.location = aURL;

  let tab = gBrowser.selectedTab;
  let browser = gBrowser.getBrowserForTab(tab);

  function onTabLoad() {
    browser.removeEventListener("load", onTabLoad, true);
    aCallback(browser, tab, browser.contentDocument);
  }

  browser.addEventListener("load", onTabLoad, true);
}

function promiseTab(aURL) {
  return new Promise(resolve =>
    addTab(aURL, resolve));
}

registerCleanupFunction(function tearDown() {
  while (gBrowser.tabs.length > 1) {
    gBrowser.removeCurrentTab();
  }

  console = undefined;
});

function catchFail(func) {
  return function() {
    try {
      return func.apply(null, arguments);
    }
    catch (ex) {
      ok(false, ex);
      console.error(ex);
      finish();
      throw ex;
    }
  };
}

/**
 * Polls a given function waiting for the given value.
 *
 * @param object aOptions
 *        Options object with the following properties:
 *        - validator
 *        A validator function that should return the expected value. This is
 *        called every few milliseconds to check if the result is the expected
 *        one. When the returned result is the expected one, then the |success|
 *        function is called and polling stops. If |validator| never returns
 *        the expected value, then polling timeouts after several tries and
 *        a failure is recorded - the given |failure| function is invoked.
 *        - success
 *        A function called when the validator function returns the expected
 *        value.
 *        - failure
 *        A function called if the validator function timeouts - fails to return
 *        the expected value in the given time.
 *        - name
 *        Name of test. This is used to generate the success and failure
 *        messages.
 *        - timeout
 *        Timeout for validator function, in milliseconds. Default is 5000 ms.
 *        - value
 *        The expected value. If this option is omitted then the |validator|
 *        function must return a trueish value.
 *        Each of the provided callback functions will receive two arguments:
 *        the |aOptions| object and the last value returned by |validator|.
 */
function waitForValue(aOptions)
{
  let start = Date.now();
  let timeout = aOptions.timeout || 5000;
  let lastValue;

  function wait(validatorFn, successFn, failureFn)
  {
    if ((Date.now() - start) > timeout) {
      // Log the failure.
      ok(false, "Timed out while waiting for: " + aOptions.name);
      let expected = "value" in aOptions ?
                     "'" + aOptions.value + "'" :
                     "a trueish value";
      info("timeout info :: got '" + lastValue + "', expected " + expected);
      failureFn(aOptions, lastValue);
      return;
    }

    lastValue = validatorFn(aOptions, lastValue);
    let successful = "value" in aOptions ?
                      lastValue == aOptions.value :
                      lastValue;
    if (successful) {
      ok(true, aOptions.name);
      successFn(aOptions, lastValue);
    } else {
      setTimeout(function() wait(validatorFn, successFn, failureFn), 100);
    }
  }

  wait(aOptions.validator, aOptions.success, aOptions.failure);
}

function oneTimeObserve(name, callback) {
  let func = function() {
    Services.obs.removeObserver(func, name);
    callback();
  };
  Services.obs.addObserver(func, name, false);
}

let createHost = Task.async(function*(type = "bottom", src = "data:text/html;charset=utf-8,") {
  let host = new Hosts[type](gBrowser.selectedTab);
  let iframe = yield host.create();

  yield new Promise(resolve => {
    let domHelper = new DOMHelpers(iframe.contentWindow);
    iframe.setAttribute("src", src);
    domHelper.onceDOMReady(resolve);
  });

  return [host, iframe.contentWindow, iframe.contentDocument];
});

/**
 * Synthesize a profile for testing.
 */
function synthesizeProfileForTest(samples) {
  const RecordingUtils = devtools.require("devtools/performance/recording-utils");

  samples.unshift({
    time: 0,
    frames: []
  });

  let uniqueStacks = new RecordingUtils.UniqueStacks();
  return RecordingUtils.deflateThread({
    samples: samples,
    markers: []
  }, uniqueStacks);
}

/**
 * Open and close the toolbox in the current browser tab, several times, waiting
 * some amount of time in between.
 * @param {Number} nbOfTimes
 * @param {Number} usageTime in milliseconds
 * @param {String} toolId
 */
function* openAndCloseToolbox(nbOfTimes, usageTime, toolId) {
  for (let i = 0; i < nbOfTimes; i++) {
    info("Opening toolbox " + (i + 1));
    let target = TargetFactory.forTab(gBrowser.selectedTab);
    yield gDevTools.showToolbox(target, toolId);

    // We use a timeout to check the toolbox's active time
    yield new Promise(resolve => setTimeout(resolve, usageTime));

    info("Closing toolbox " + (i + 1));
    yield gDevTools.closeToolbox(target);
  }
}

/**
 * Synthesize a profile for testing.
 */
function synthesizeProfileForTest(samples) {
  const RecordingUtils = require("devtools/shared/performance/utils");

  samples.unshift({
    time: 0,
    frames: []
  });

  let uniqueStacks = new RecordingUtils.UniqueStacks();
  return RecordingUtils.deflateThread({
    samples: samples,
    markers: []
  }, uniqueStacks);
}

/**
 * Waits until a predicate returns true.
 *
 * @param function predicate
 *        Invoked once in a while until it returns true.
 * @param number interval [optional]
 *        How often the predicate is invoked, in milliseconds.
 */
function waitUntil(predicate, interval = 10) {
  if (predicate()) {
    return Promise.resolve(true);
  }
  return new Promise(resolve => {
    setTimeout(function() {
      waitUntil(predicate).then(() => resolve(true));
    }, interval);
  });
}

/**
 * Show the presets list sidebar in the cssfilter widget popup
 * @param {CSSFilterWidget} widget
 * @return {Promise}
 */
function showFilterPopupPresets(widget) {
  let onRender = widget.once("render");
  widget._togglePresets();
  return onRender;
}

/**
 * Show presets list and create a sample preset with the name and value provided
 * @param  {CSSFilterWidget} widget
 * @param  {string} name
 * @param  {string} value
 * @return {Promise}
 */
var showFilterPopupPresetsAndCreatePreset = Task.async(function*(widget, name, value) {
  yield showFilterPopupPresets(widget);

  let onRender = widget.once("render");
  widget.setCssValue(value);
  yield onRender;

  let footer = widget.el.querySelector(".presets-list .footer");
  footer.querySelector("input").value = name;

  onRender = widget.once("render");
  widget._savePreset({
    preventDefault: () => {}
  });

  yield onRender;
});

/**
 * Utility function for testing CSS code samples that have been
 * syntax-highlighted.
 *
 * The CSS syntax highlighter emits a collection of DOM nodes that have
 * CSS classes applied to them. This function checks that those nodes
 * are what we expect.
 *
 * @param {array} expectedNodes
 * A representation of the nodes we expect to see.
 * Each node is an object containing two properties:
 * - type: a string which can be one of:
 *   - text, comment, property-name, property-value
 * - text: the textContent of the node
 *
 * For example, given a string like this:
 * "<comment> The part we want   </comment>\n this: is-the-part-we-want;"
 *
 * we would represent the expected output like this:
 * [{type: "comment",        text: "<comment> The part we want   </comment>"},
 *  {type: "text",           text: "\n"},
 *  {type: "property-name",  text: "this"},
 *  {type: "text",           text: ":"},
 *  {type: "text",           text: " "},
 *  {type: "property-value", text: "is-the-part-we-want"},
 *  {type: "text",           text: ";"}];
 *
 * @param {Node} parent
 * The DOM node whose children are the output of the syntax highlighter.
 */
function checkCssSyntaxHighlighterOutput(expectedNodes, parent) {
  /**
   * The classes applied to the output nodes by the syntax highlighter.
   * These must be same as the definitions in MdnDocsWidget.js.
   */
  const PROPERTY_NAME_COLOR = "theme-fg-color5";
  const PROPERTY_VALUE_COLOR = "theme-fg-color1";
  const COMMENT_COLOR = "theme-comment";

  /**
   * Check the type and content of a single node.
   */
  function checkNode(expected, actual) {
    ok(actual.textContent == expected.text, "Check that node has the expected textContent");
    info("Expected text content: [" + expected.text + "]");
    info("Actual text content: [" + actual.textContent + "]");

    info("Check that node has the expected type");
    if (expected.type == "text") {
      ok(actual.nodeType == 3, "Check that node is a text node");
    } else {
      ok(actual.tagName.toUpperCase() == "SPAN", "Check that node is a SPAN");
    }

    info("Check that node has the expected className");

    let expectedClassName = null;
    let actualClassName = null;

    switch (expected.type) {
      case "property-name":
        expectedClassName = PROPERTY_NAME_COLOR;
        break;
      case "property-value":
        expectedClassName = PROPERTY_VALUE_COLOR;
        break;
      case "comment":
        expectedClassName = COMMENT_COLOR;
        break;
      default:
        ok(!actual.classList, "No className expected");
        return;
    }

    ok(actual.classList.length == 1, "One className expected");
    actualClassName = actual.classList[0];

    ok(expectedClassName == actualClassName, "Check className value");
    info("Expected className: " + expectedClassName);
    info("Actual className: " + actualClassName);
  }

  info("Logging the actual nodes we have:");
  for (var j = 0; j < parent.childNodes.length; j++) {
    var n = parent.childNodes[j];
    info(j + " / " +
         "nodeType: "+ n.nodeType + " / " +
         "textContent: " + n.textContent);
  }

  ok(parent.childNodes.length == parent.childNodes.length,
    "Check we have the expected number of nodes");
  info("Expected node count " + expectedNodes.length);
  info("Actual node count " + expectedNodes.length);

  for (let i = 0; i < expectedNodes.length; i++) {
    info("Check node " + i);
    checkNode(expectedNodes[i], parent.childNodes[i]);
  }
}
