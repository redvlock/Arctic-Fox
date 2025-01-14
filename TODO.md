# Backlog of Mozilla patches:
(grossly ordered in dependency order, not always correct, oldest to work on at the bottom)

- Bug 1633339 - Fix infinite loop when network changes and e10s is disabled.
- Bug 1533969 - Fix build error with newer glibc. (gettid)
- Bug 1530958 (CVE-2019-9791) Spidermonkey: IonMonkey's type inference is incorrect for constructors entered via OSR
- Bug 1499277 - Remove unnecessary SCInput::readNativeEndian; fix SCInput::readPtr on big endian systems. r=sfink
- 1499861 - issues when backporting on other collections
- 1477632 - Always inline PLDHashTable::SearchTable(
- 1472925 - keep a strong reference to MediaStreamGraph from GraphDriver
- 1470260 - part 2 - Make RefreshDriverTimer ref-counted and hold a s
- 1470260 - part 1 - Ensure that 'this' stays alive for the duration
- 1472018 - Limit the lock scope in WebCryptoThreadPool::Shutdown.
- Bug 1464751 2018-05-28 If there is no JIT, there should be no JIT signal handlers
- 1469309 - Remove an unused sensor type
- Bug 1443943 Allow internal callers of performance.now() to opt-out of
- Bug 1443943 - 2018-03-09 - Port the performance APIs only to only clamping/jittering
- Bug 1420092 -2017-11-23 - Don't always enable mozjemalloc by default when building the js engine. r=njn
- Bug 1423875. Fix InitializePropertiesFromCompatibleNativeObject to ge
- 1419960 - Make the noopener window feature not affect whether oth
- 1381728 - Part 1 : <object data="data:text/html",...> should have
- 1412081 - Call KillClearOnShutdown(ShutdownPhase::ShutdownFinal)
- 1412081 - Add ability to blacklist file paths on Unix platforms 
- 1364624 - Switch from CRITICALSECTION to SRWLOCK (Windows, 2 parts)
- 1358469 - Revert our web-incompatible change to rel=noreferrer tar
- 1352874 - Improve nsHtml5AtomTable performance
- 1342849 - Don't do any notifications for newly added background t
- 1324406 - Treat 'data:' documents as unique, opaque origins
- 1300118 P1 Make TaskQueue deliver runnables to nsIEventTarget
- Add d3d11/d2d and compositor information to telemetry. (bug 1179051
- Bug 1184705 - Search A/B testing cohort identifier should be recorded
- Add layers.offmainthreadcomposition.enabled to telemetry. (bug 1187453
- Add a "blacklisted" property to d3d11 telemetry. (bug 1187453, 
-  Add compositor, layers, and rendering info to nsIGfxInfo. (bug 1179051 part 5, r=mattwoodrow)\
- Split gfxWindowsPlatform::UpdateRenderMode() into multiple functions.  (bug 1179051 part 1, r=bas)
- Bug 1379957 - 2017-07-12  - Only fire the debugger's onGarbageCollection hook when
- Bug 1362167 - 2017-05-04 - Use strongly-typed enum classes instead of generic uint
- Bug 1352528 - 2017-04-03 - Hoist call to GetRoundOffsetsToPixels out of the inner 
- 1297276 - Rename mfbt/unused.h to mfbt/Unused.h for consistency
- 1276938 - Optimize string usage in setAttribute when dealing with
- 1263778 - Rename a bunch of low-level [[Prototype]] access methods to make their interactions with statically-known and dynamically-computed [[Prototype]]s clearer : Too much work for now
- 1258205 - Make setAttribute throw InvalidCharacterError if the
- 1244098 - fold jspo_in, would improve performance, but we are missing testNotDefinedProperty and for that we need shouldAbortOnPreliminaryGroups() and that needs preliminaryObjects in the ObjectGroup
- 1235656 - Set canonical name in self-hosted builtins
- 1223690 - Remove implicit Rect conversions
- 1222516 - 2016-10-20 part 4. Implement support for rel=noopener on links. - apply part3 before
- Bug 1310721 - 2016-10-15- Remove support for the b2g PAC generator; r=mcmanus
- 1222516 part 3. Rejigger our rel="noreferrer" - unable to apply because of inherit principal vs inherit owner, furthermore nsNullPtr
- Bug 1184130. Report mismatches of adapter description and vendor id t
- Bug 1159751: Ensure WARP can never be used for Windows 7. r=milan 
- Bug 1178426. Add GfxInfo to ServicesList.h. r=nfroyd 
- Bug 1279303 - 2017-07-27 - Implement change to O.getOwnPropertyDescriptors and upd
- 1114580 - toStringTag - several diffs still to analyze
- Bug 1278838 2016-06-09- Remove separate worker binding for Performance API
- Bug 1245024 - 2016-06-09 - Implement Object.getOwnPropertyDescriptors. r=efaust,bz (check https://forum.manjaro.org/ still works after applying)
- Bug 1266391 - 2016-04-21 Introduce an enum class mozilla::unicode::Script, and u
- Bug 1209100 - 2016-03-21 - Back out bug 1165185 on inbound.
- Bug 1296851 - 2016-10-27 Always allow SetPrototype with the same value as the cu
- Bug 1263778 - 2016-03-19 Rename a bunch of low-level [[Prototype]] access method
- Bug 1255511 - 2016-03-15 Skip beforeunload prompts once nsIAppStartup shuttingDo
- Bug 1258905 - 2016-03-28 Remove a bunch of dead IPC code.
- Bug 1252262 - 2016-03-08 - Don't combine the client offset into the outer rect for
- Bug 1249787 - 2016-02-20 - BaldrMonkey: Fix wasm string hex escape parsing endiann
- 2016-02-21 Back out 3 changesets (bug 1216001) for Win8 reftest failures in 1193� 
- Bug 1251347 - Refining SessionFile Shutdown hang details;r
- Bug 1251347 - Making sure that SessionFile.write initializes its work
- Bug 1244650 - Failure to clear Forms and Search Data on exit. r
- Bug 1243549 - Add missing bits. r=post-facto 
- Bug 612168 [recheck existing] - Handle uninstalls of restartless addons in XPIProvider
- Bug 1243549 - 2016-02-04 Make sure that startup sanitization doesn't throw
- Bug 1219339 - 2016-01-14 : switch GetStaticInstance to use IPC's Singleton<T>
- 1219392 - Capitalize mozilla::unused to avoid conflicts
- Bug 1219339 - 2016-10-02 Part2: Ensure close of webrtc trace file during shutdow
- Bug 1295729 - 2016-08-16 - Ensure that properties are array indices when the conso
- 1079844 - Refer to "detaching" instead of "neutering" of ArrayBuf
- Bug 1238290 - 2016-01-09 - fix bad necko deps on unified_sources r=valentin.gosu
- 1245241 remaining 4 parts
- Bug 1245241 - 2016-02-18 - part 1 - Close Shmem file handles after mapping them wh
- 1164427 - Implement elementsFromPoint (= Tests)
- Bug 1230948 - Update web-platform-tests expected data to revision 63b
- Bug 1160971 - 4 parts
- Bug 1233176 - 2015-12-22 - Scalar Replacement: Initialize properties with the defa
- Bug 1231109 - Drop FreeBSD checks for unsupported versions. r=jld
- rest of 1198458
- Bug 1198458: Rollup of changes previously applied to media/webrtc/tru
- Bug 1198458: Webrtc updated to branch 43
- 1227567 - Optimise module namespace imports in Ion where we have
- update SKIA 1082598
- remaining 1151214
- Bug 1177310 - 2015-11-25- TabStateFlusher Promises should always resolve.
- Bug 1175609 - 2015-11-17 - Bring onnegotiationneeded in line with spec. r=mt
- Bug 1197620 - Part 3: Terminate *all* animations if corresponding ele
- Bug 1235656 - Followup: Allow extended functions with guessed atoms i
- Bug 1235656 - Part 2: Remove alias to selfhosted builtin from Utiliti
- Bug 1223916 - 2015-11-14 Prohibit direct method calls at the parser level in 
- Bug 1223647: CSP erroneously inherited into dedicated workers. r=cke
- Bug 1213859 - Focus and blur events should not be cancelable; r=smaug
- Bug 1211546 - Unbreak the non-unified build. (r=sfink, r=nbp, r=shu) 
- Bug 1160307 - 2015-11-05 - capture async stack frames on Javascript timeline marke
- Bug 1169268 - 2015-10-27 - Don't crash when pasting files. r=ndeakin 
- Bug 1039986 - 2015-10-27 - (Fix cloudflare?)  Make Function.prototype.toString work on Web IDL interfa
- Bug 930414 - Replace |thisObject| object op with |thisValue| and us
- Bug 1238935 - r=jonco 
- Bug 1214126 - 5 Parts
- Bug 1216227 - 2015-10-20 - do bucketed page-load-per-window counts to assess table
- Bug 1208385 part 1 - Store a pointer to the owning animation on each � 
- 1214508 - SharedStubs - Part 3: Enable the getprop stubs in ionmon
- Bug 1214508: SharedStubs - Part 1: Move the getprop stubs in to share
- Bug 1232269 - 2015-12-22 - Use the correct receiver when calling an own getter or
- Bug 1257650 - 2016-03-19 Skip Security checks if triggeringPrincipal is System
- Bug 1232903 - Skip Security checks if triggeringPrincipal is SystemPr
- Bug 1226909 multipart AsyncOpen2 
- Bug 1213646: Allow URI_IS_UI_RESOURCE and safe about: URIs when SEC_A
- 1185106 - at least part 0 to 4 for TFF
- Check completeness of Bug 1216401: Eviscerate nsIDOMWindow
- Bug 1158632 - IonBuilder TableSwitch, fill-the-gap cases should encor�
- Bug 1212129 - (2015-10-22 partialy applied) e10s support for disabling site notifications. r=wchen
- Bug 1182571 - 2015-10-19 Fix nsILoadInfo->GetContentPolicyType API to be less am
- Bug 1170958 - 2015-09-30 - Allow MediaInputPort to lock to a specific input track
- 1203427
- 1207245 - 2015-10-07 part 6 - rename nsRefPtr<T> to RefPtr<T>
- Bug 1169268 - Don't crash when pasting files. r=ndeakin
- Bug 1202085 2015-10-26 - Part 0 to 6
- Bug 1188643 2015-09-30 - Buffer more audio in audio capture mode to avoid glitche
- https://bugzilla.mozilla.org/show_bug.cgi?id=1201314
- Bug 1257468 - Replace tests on BUILDING_JS with tests on MOZ_BUILD_AP
- bug 1244743 - Replace MOZ_NATIVE_X with MOZ_SYSTEM_X. 
- Bug 1214163 - 2015-10-15 - Clean up SetPropertyIC::update. r=efaust
- bug 1181823 - convert test_ev_certs.js, test_keysize_ev.js, a
- 1207245 - part 3 - switch all uses of mozilla::RefPtr<T> to nsRefPtr<T>
- Bug 1242578
-  missing bits of Bug 1207245 - part 1 - move RefCounted<T>

JS - breaks:
https://bugzilla.mozilla.org/show_bug.cgi?id=589199


impacting download and shutdown:
Bug 875648 - Use Downloads.jsm functions to get download directories


More session store stuff to check:

- Bug 1243549 - Add missing bits. r=post-facto
- Bug 1243549 - Make sure that startup sanitization doesn't throw becau

ARM fixes to check
- https://bugzilla.mozilla.org/show_bug.cgi?id=1179514

# Build System - not working
Bug 1137364 - part 2 - move browser themes icon installation to FINAL

Check with Roy Tam:
- Bug 1129633 - part 2. In prefs, set win8 provider to RELEASE-only. 
- Bug 1129633 - part1. Use win8 geolocation with a fallback to MLS
- bug 1139012 - telemetry for MLS vs win8 geolocation response.

Parents of:
https://github.com/mozilla/newtab-dev/commit/ac250f9d737362d0730b0897603ae379eca89ebf

## Enhancing JS
 Bug 1316079 - Mark JS::PropertyDescriptor as JS_PUBLIC_API to fix lin�
- Bug 1208747 - Move most of Stopwatch-related code to XPCOM-land (JSAP
- Bug 930414 - remaining 6 out of 22 parts

### FIXME / TODO
- Reapply Bug 486262 - Part 2 with removal of tabbrowser.dtd - breaks browser currently
- fix devtools structure, from browser/themes/osx/devtools to browser/devtools
Specifically check for duplicates:
  browser/themes/osx/devtools/server
  browser/themes/osx/devtools/shared/inspector/

Shell Service not working? present but fails.
Check TelemetryEnvironment.jsm _isDefaultBrowser


Remove hack of parserequestcontenttype in nsNetUtil.cpp


Fallible hacks:
appendElements made fallible when not so in original FF:
media/libstagefright/frameworks/av/media/libstagefright/MPEG4Extractor.cpp

### Further ToDo which would help portability:

- from nsContextMenu.js : remove unremotePrincipal again

- Update UniquePtr
- in nsGlobalWindow remove from Open calls aCalleePrincipal and aJSCallerContext
- inherit principal vs. inherit owner in DocShell see INTERNAL_LOAD_FLAGS_INHERIT_OWNER
- update nsNullPrincipal (and nsDocShell Fixme's)
- add PrincipalToInherit to LoadInfo
- LoadFrame needs TriggerPrincipal & OriginalSrc
- move SharedThreadPool from domi/media to xpcom/threads
- complete 1487964 port
- check bugs: bug 1275755, bug 1352874, bug 1440824 as prerequisites for Bug 529808


### Further Further ToDo:
- Check for STLPort removal: https://bugzilla.mozilla.org/show_bug.cgi?id=1276927
- import PPC JIT from TenFourFox
- flatten out security manager ssl
- factor out dom/base/nsGlobalWindowInner.cpp
- NekcoOriginAttributes
- see if window.requestIdleCallback can be backported

### last checked TFF backport commit
#512: M1472018 M1469309 M1472925 M1470260 (part 1)

-- consider non taken bugs for platforms we do support compared to TFF (and update list here)
https://github.com/classilla/tenfourfox/issues/526

## JS Sputnik checks:

2018-12-10:
* Full: Tests To run: 16436 | Total tests ran: 6976 | Pass: 6048 | Fail: 928 | Failed to load: 0 - Hangs on "iter-close"
* Harness: Tests To run: 55 | Total tests ran: 55 | Pass: 55 | Fail: 0 | Failed to load: 0
* Language: Tests To run: 5052 | Total tests ran: 5052 | Pass: 4452 | Fail: 600 | Failed to load: 0
* AnnexB: Tests To run: 81 | Total tests ran: 81 | Pass: 79 | Fail: 2 | Failed to load: 0

2021-09-13:
* Full: Tests To run: 16436 | Total tests ran: 6976 | Pass: 6095 | Fail: 881 | Failed to load: 0
* Harness: Tests To run: 55 | Total tests ran: 55 | Pass: 55 | Fail: 0 | Failed to load: 0
* Language: Tests To run: 5052 | Total tests ran: 5052 | Pass: 4466 | Fail: 586 | Failed to load: 0
* AnnexB: Tests To run: 81 | Total tests ran: 81 | Pass: 79 | Fail: 2 | Failed to load: 0
