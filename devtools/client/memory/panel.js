/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
"use strict";

const { Cc, Ci, Cu, Cr } = require("chrome");
const { Task } = require("resource://gre/modules/Task.jsm");
const EventEmitter = require("devtools/shared/event-emitter");
const { MemoryFront } = require("devtools/server/actors/memory");
const promise = require("promise");

function MemoryPanel (iframeWindow, toolbox) {
  this.panelWin = iframeWindow;
  this._toolbox = toolbox;

  EventEmitter.decorate(this);
}

MemoryPanel.prototype = {
  open: Task.async(function *() {
    if (this._opening) {
      return this._opening;
    }

    this.panelWin.gToolbox = this._toolbox;
    this.panelWin.gTarget = this.target;

    const rootForm = yield this.target.root;
    this.panelWin.gFront = new MemoryFront(this.target.client,
                                           this.target.form,
                                           rootForm);

    console.log(this.panelWin, this.panelWin.MemoryController);
    this._opening = this.panelWin.MemoryController.initialize().then(() => {
      this.isReady = true;
      this.emit("ready");
      return this;
    });
    return this._opening;
  }),

  // DevToolPanel API

  get target() {
    return this._toolbox.target;
  },

  destroy: function () {
    // Make sure this panel is not already destroyed.
    if (this._destroyer) {
      return this._destroyer;
    }

    this._destroyer = this.panelWin.MemoryController.destroy().then(() => {
      // Destroy front to ensure packet handler is removed from client
      this.panelWin.gFront.destroy();
      this.panelWin = null;
      this.emit("destroyed");
      return this;
    });
    return this._destroyer;
  }
};

exports.MemoryPanel = MemoryPanel;
