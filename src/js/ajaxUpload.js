function tweakFormCode(autoSubmit, iframeId, submitId, maxFileSize) {
    var subm = document.getElementById(submitId);
    if (subm == null && thisScript != null && thisScript.parentNode != null && thisScript.parentNode.getElementsByTagName) {
        var subms = thisScript.parentNode.getElementsByTagName("input");
        for (var i = 0; i < subms.length; ++i)
            if (subms[i].id == submitId) {
                subm = subms[i];
                break;
            }
    }
    if (subm == null) er("Can't find AjaxUpload control!");

    subm.parentNode.target = iframeId;

    if (subm.begin == undefined) {
        subm.begin = subm.onfocus;
        subm.onfocus = undefined;
    }

    subm.parentNode.onsubmit = function() {
        subm.begin();
        return true;
    };

    if (subm.withHandle == undefined) {
        subm.withHandle = subm.onkeydown;
        subm.onkeydown = undefined;
    }

    if (subm.withMimeTypeError == undefined) {
        subm.withMimeTypeError = subm.onblur;
        subm.onblur = undefined;
    }
    if (subm.withFileSizeError == undefined) {
      subm.withFileSizeError = subm.onkeyup;
      subm.onkeyup = undefined;
    }

    function checkFileSize(ev){
      if (ev.target && ev.target.files && ev.target.files[0] && ev.target.files[0].size > maxFileSize){
        subm.withFileSizeError(ev);
        return false;
      }
      return true;
    }
    for (var node = subm.previousSibling; node.tagName != "INPUT"; node = node.previousSibling);
    node.onchange = function(ev){
      checkFileSize(ev);
    };

    if (autoSubmit) {
        subm.style.visibility = "hidden";
        node.onchange = function(ev) {
          var ok = checkFileSize(ev);
          if (ok) {
            subm.parentNode.submit();
            if (subm.begin){
              subm.begin(); // calling submit manually doesn't emit onsubmit event
            }
          };
        };
    }
}

function tweakForm(autoSubmit, iframeId, submitId, maxFileSize) {
    return "<iframe id=\""
        + iframeId
        + "\" name=\""
        + iframeId
        + "\" src=\"\" style=\"width:0;height:0;border:0px solid #fff;\"></iframe>\n<script type=\"text/javascript\">tweakFormCode("
        + autoSubmit
        + ",\""
        + iframeId
        + "\",\""
        + submitId
        + "\",\""
        + maxFileSize
        + "\");</script>";
}

function idToString(x) {
    return x;
}
