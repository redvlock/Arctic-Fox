# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

Cu.import("resource://gre/modules/XPCOMUtils.jsm");

#ifdef MOZ_SERVICES_CLOUDSYNC
XPCOMUtils.defineLazyModuleGetter(this, "CloudSync",
                                  "resource://gre/modules/CloudSync.jsm");
#else
var CloudSync = null;
#endif

// gSyncUI handles updating the tools menu and displaying notifications.
var gSyncUI = {
  _obs: ["weave:service:sync:start",
         "weave:service:sync:finish",
         "weave:service:sync:error",
         "weave:service:setup-complete",
         "weave:service:login:start",
         "weave:service:login:finish",
         "weave:service:login:error",
         "weave:service:logout:finish",
         "weave:service:start-over",
         "weave:service:start-over:finish",
         "weave:ui:login:error",
         "weave:ui:sync:error",
         "weave:ui:sync:finish",
         "weave:ui:clear-error",
  ],

  _unloaded: false,
  // The number of "active" syncs - while this is non-zero, our button will spin
  _numActiveSyncTasks: 0,

  init: function () {
    Cu.import("resource://services-common/stringbundle.js");

    // Proceed to set up the UI if Sync has already started up.
    // Otherwise we'll do it when Sync is firing up.
    let xps = Components.classes["@mozilla.org/weave/service;1"]
                                .getService(Components.interfaces.nsISupports)
                                .wrappedJSObject;
    if (xps.ready) {
      this.initUI();
      return;
    }

    Services.obs.addObserver(this, "weave:service:ready", true);

    // Remove the observer if the window is closed before the observer
    // was triggered.
    window.addEventListener("unload", function onUnload() {
      gSyncUI._unloaded = true;
      window.removeEventListener("unload", onUnload, false);
      Services.obs.removeObserver(gSyncUI, "weave:service:ready");

      if (Weave.Status.ready) {
        gSyncUI._obs.forEach(function(topic) {
          Services.obs.removeObserver(gSyncUI, topic);
        });
      }
    }, false);
  },

  initUI: function SUI_initUI() {
    // If this is a browser window?
    if (gBrowser) {
      this._obs.push("weave:notification:added");
    }

    this._obs.forEach(function(topic) {
      Services.obs.addObserver(this, topic, true);
    }, this);

    if (gBrowser && Weave.Notifications.notifications.length) {
      this.initNotifications();
    }
    this.updateUI();
  },

  initNotifications: function SUI_initNotifications() {
    const XULNS = "http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul";
    let notificationbox = document.createElementNS(XULNS, "notificationbox");
    notificationbox.id = "sync-notifications";
    notificationbox.setAttribute("flex", "1");

    let bottombox = document.getElementById("browser-bottombox");
    bottombox.insertBefore(notificationbox, bottombox.firstChild);

    // Force a style flush to ensure that our binding is attached.
    notificationbox.clientTop;

    // notificationbox will listen to observers from now on.
    Services.obs.removeObserver(this, "weave:notification:added");
  },

  _wasDelayed: false,

  _needsSetup() {
    // We want to treat "account needs verification" as "needs setup". So
    // "reach in" to Weave.Status._authManager to check whether we the signed-in
    // user is verified.
    // NOTE: We used to have this _authManager hack to avoid a nested
    // event-loop from querying Weave.Status.checkSetup() - while that's no
    // longer true, we do still have the FxA-specific requirement of checking
    // the verified state - so the hack remains. We should consider refactoring
    // Sync's "check setup" capabilities to take this into account at some point...
    if (Weave.Status._authManager._signedInUser !== undefined) {
      // If we have a signed in user already, and that user is not verified,
      // revert to the "needs setup" state.
      return !Weave.Status._authManager._signedInUser ||
             !Weave.Status._authManager._signedInUser.verified;
    }

    // We are using legacy sync - check that.
    let firstSync = "";
    try {
      firstSync = Services.prefs.getCharPref("services.sync.firstSync");
    } catch (e) { }
    return Weave.Status.checkSetup() == Weave.CLIENT_NOT_CONFIGURED ||
           firstSync == "notReady";
  },

  _needsVerification() {
    // For callers who care about the distinction between "needs setup" and
    // "setup but needs verification"
    // See _needsSetup for the subtleties here.
    if (Weave.Status._authManager._signedInUser === undefined) {
      // a legacy sync user - no "verified" concept there.
      return false;
    }
    if (!Weave.Status._authManager._signedInUser) {
      // no user configured at all, so not in a "need verification" state.
      return false;
    }
    return !Weave.Status._authManager._signedInUser.verified;
  },

  // Note that we don't show login errors in a notification bar here, but do
  // still need to track a login-failed state so the "Tools" menu updates
  // with the correct state.
  _loginFailed: function () {
    this.log.debug("_loginFailed has sync state=${sync}",
                   { sync: Weave.Status.login});
    return Weave.Status.login == Weave.LOGIN_FAILED_LOGIN_REJECTED;
  },

  updateUI: function SUI_updateUI() {
    let needsSetup = this._needsSetup();
    let loginFailed = this._loginFailed();

    // Start off with a clean slate
    document.getElementById("sync-reauth-state").hidden = true;
    document.getElementById("sync-setup-state").hidden = true;
    document.getElementById("sync-syncnow-state").hidden = true;

    if (CloudSync && CloudSync.ready && CloudSync().adapters.count) {
      document.getElementById("sync-syncnow-state").hidden = false;
    } else if (loginFailed) {
      // unhiding this element makes the menubar show the login failure state.
      document.getElementById("sync-reauth-state").hidden = false;
    } else if (needsSetup) {
      document.getElementById("sync-setup-state").hidden = false;
    } else {
      document.getElementById("sync-syncnow-state").hidden = false;
    }

    this._updateSyncButtonsTooltip();
  },

  // Functions called by observers
  onActivityStart() {
    if (!gBrowser)
      return;

    this.log.debug("onActivityStart with numActive", this._numActiveSyncTasks);
    if (++this._numActiveSyncTasks == 1) {
      let button = document.getElementById("sync-button");
      if (button) {
        button.setAttribute("status", "active");
      }
      button = document.getElementById("PanelUI-fxa-status");
      if (button) {
        button.setAttribute("syncstatus", "active");
      }
    }
    this.updateUI();
  },

  onActivityStop() {
    if (!gBrowser)
      return;
    this.log.debug("onActivityStop with numActive", this._numActiveSyncTasks);
    if (--this._numActiveSyncTasks) {
      if (this._numActiveSyncTasks < 0) {
        // This isn't particularly useful (it seems more likely we'll set a
        // "start" without a "stop" meaning it forever remains > 0) but it
        // might offer some value...
        this.log.error("mismatched onActivityStart/Stop calls",
                       new Error("active=" + this._numActiveSyncTasks));
      }
      return; // active tasks are still ongoing...
    }

    let syncButton = document.getElementById("sync-button");
    if (syncButton) {
      syncButton.removeAttribute("status");
    }
    let fxaContainer = document.getElementById("PanelUI-footer-fxa");
    if (fxaContainer) {
      fxaContainer.removeAttribute("syncstatus");
    }
    this.updateUI();
  },

  onLoginError: function SUI_onLoginError() {
    this.log.debug("onLoginError: login=${login}, sync=${sync}", Weave.Status);
    Weave.Notifications.removeAll();

    // We don't show any login errors here; browser-fxaccounts shows them in
    // the hamburger menu.
    this.updateUI();
  },

  onLogout: function SUI_onLogout() {
    this.updateUI();
  },

  onStartOver: function SUI_onStartOver() {
    this.clearError();
  },

  _getAppName: function () {
    let brand = new StringBundle("chrome://branding/locale/brand.properties");
    return brand.get("brandShortName");
  },

  // Commands
  doSync: function SUI_doSync() {
    setTimeout(function() Weave.Service.errorHandler.syncAndReportErrors(), 0);
  },

  handleToolbarButton: function SUI_handleStatusbarButton() {
    if (this._needsSetup())
      this.openSetup();
    else
      this.doSync();
  },

  /**
   * Invoke the Sync setup wizard.
   *
   * @param wizardType
   *        Indicates type of wizard to launch:
   *          null    -- regular set up wizard
   *          "pair"  -- pair a device first
   *          "reset" -- reset sync
   */

  openSetup: function SUI_openSetup(wizardType, entryPoint = "syncbutton") {
    let win = Services.wm.getMostRecentWindow("Weave:AccountSetup");
    if (win)
      win.focus();
    else {
      window.openDialog("chrome://browser/content/sync/setup.xul",
                        "weaveSetup", "centerscreen,chrome,resizable=no",
                        wizardType);
    }
  },

  openAddDevice: function () {
    if (!Weave.Utils.ensureMPUnlocked())
      return;

    let win = Services.wm.getMostRecentWindow("Sync:AddDevice");
    if (win)
      win.focus();
    else
      window.openDialog("chrome://browser/content/sync/addDevice.xul",
                        "syncAddDevice", "centerscreen,chrome,resizable=no");
  },

  openPrefs: function SUI_openPrefs() {
    openPreferences("paneSync");
  },


  /* Update the tooltip for the Sync Toolbar button and the Sync spinner in the
     FxA hamburger area.
     If Sync is configured, the tooltip is when the last sync occurred,
     otherwise the tooltip reflects the fact that Sync needs to be
     (re-)configured.
  */
  _updateSyncButtonsTooltip: function() {
    if (!gBrowser)
      return;

    let syncButton = document.getElementById("sync-button");
    if (!syncButton)
      return;

    let email;
    try {
      email = Services.prefs.getCharPref("services.sync.username");
    } catch (ex) {}

    // This is a little messy as the Sync buttons are 1/2 Sync related and
    // 1/2 FxA related - so for some strings we use Sync strings, but for
    // others we reach into gFxAccounts for strings.
    let tooltiptext;
    if (this._needsVerification()) {
      // "needs verification"
      tooltiptext = gFxAccounts.strings.formatStringFromName("verifyDescription", [email], 1);
    } else if (this._needsSetup()) {
      // "needs setup".
      tooltiptext = this._stringBundle.GetStringFromName("signInToSync.description");
    } else if (this._loginFailed()) {
      // "need to reconnect/re-enter your password"
      tooltiptext = gFxAccounts.strings.formatStringFromName("reconnectDescription", [email], 1);
    } else {
      // Sync appears configured - format the "last synced at" time.
      try {
        let lastSync = new Date(Services.prefs.getCharPref("services.sync.lastSync"));
        // Show the day-of-week and time (HH:MM) of last sync
        let lastSyncDateString = lastSync.toLocaleFormat("%a %H:%M");
        tooltiptext = this._stringBundle.formatStringFromName("lastSync2.label", [lastSyncDateString], 1);
      }
      catch (e) {
        // pref doesn't exist (which will be the case until we've seen the
        // first successful sync) or is invalid (which should be impossible!)
        // Just leave tooltiptext as the empty string in these cases, which
        // will cause the tooltip to be removed below.
      }
    }

    for (let button of [syncButton, statusButton]) {
      if (button) {
        if (tooltiptext) {
          button.setAttribute("tooltiptext", tooltiptext);
        } else {
          button.removeAttribute("tooltiptext");
        }
      }
    }
  },

  clearError: function SUI_clearError(errorString) {
    Weave.Notifications.removeAll(errorString);
    this.updateUI();
  },

  onSyncFinish: function SUI_onSyncFinish() {
    let title = this._stringBundle.GetStringFromName("error.sync.title");

    // Clear out sync failures on a successful sync
    this.clearError(title);

    if (this._wasDelayed && Weave.Status.sync != Weave.NO_SYNC_NODE_FOUND) {
      title = this._stringBundle.GetStringFromName("error.sync.no_node_found.title");
      this.clearError(title);
      this._wasDelayed = false;
    }
  },

  observe: function SUI_observe(subject, topic, data) {
    this.log.debug("observed", topic);
    if (this._unloaded) {
      Cu.reportError("SyncUI observer called after unload: " + topic);
      return;
    }

    // Unwrap, just like Svc.Obs, but without pulling in that dependency.
    if (subject && typeof subject == "object" &&
        ("wrappedJSObject" in subject) &&
        ("observersModuleSubjectWrapper" in subject.wrappedJSObject)) {
      subject = subject.wrappedJSObject.object;
    }

    // First handle "activity" only.
    switch (topic) {
      case "weave:service:sync:start":
      case "weave:service:login:start":
        this.onActivityStart();
        break;
      case "weave:service:sync:finish":
      case "weave:service:sync:error":
      case "weave:service:login:finish":
      case "weave:service:login:error":
        this.onActivityStop();
        break;
    }
    // Now non-activity state (eg, enabled, errors, etc)
    // Note that sync uses the ":ui:" notifications for errors because sync.
    switch (topic) {
      case "weave:ui:sync:finish":
        this.onSyncFinish();
        break;
      case "weave:ui:sync:error":
      case "weave:service:sync:delayed":
        this.onSyncDelay();
        break;
      case "weave:service:setup-complete":
        this.updateUI();
        break;
      case "weave:ui:login:error":
        this.onLoginError();
        break;
      case "weave:service:logout:finish":
        this.onLogout();
        break;
      case "weave:service:start-over":
        this.onStartOver();
        break;
      case "weave:service:start-over:finish":
        this.updateUI();
        break;
      case "weave:service:ready":
        this.initUI();
        break;
      case "weave:notification:added":
        this.initNotifications();
        break;
      case "weave:ui:clear-error":
        this.clearError();
        break;
    }
  },

  QueryInterface: XPCOMUtils.generateQI([
    Ci.nsIObserver,
    Ci.nsISupportsWeakReference
  ])
};

XPCOMUtils.defineLazyGetter(gSyncUI, "_stringBundle", function() {
  //XXXzpao these strings should probably be moved from /services to /browser... (bug 583381)
  //        but for now just make it work
  return Cc["@mozilla.org/intl/stringbundle;1"].
         getService(Ci.nsIStringBundleService).
         createBundle("chrome://weave/locale/services/sync.properties");
});

XPCOMUtils.defineLazyGetter(gSyncUI, "log", function() {
  return Log.repository.getLogger("browserwindow.syncui");
});
