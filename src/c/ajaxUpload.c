#include <ctype.h>

#include <ajaxUpload.h>

uw_Basis_string uw_AjaxUploadFfi_tweakForm(uw_context ctx, uw_Basis_bool autoSubmit, uw_Basis_string iframeId, uw_Basis_string submitId, uw_Basis_int maxFileSize) {
  return uw_Basis_mstrcat(ctx,
                          "<iframe id=\"",
                          iframeId,
                          "\" name=\"",
                          iframeId,
                          "\" src=\"#\" style=\"width:0;height:0;border:0px solid #fff;\"></iframe>\n<script type=\"text/javascript\">var subm = document.getElementById(\"",
                          submitId,
                          "\"); subm.parentNode.target = \"",
                          iframeId,
                          "\"; subm.begin = subm.onfocus; subm.onfocus = undefined; subm.parentNode.onsubmit = function() { window.top.event = {}; subm.begin(); return true; }; subm.withHandle = subm.onkeydown; subm.onkeydown = undefined; subm.withMimeTypeError = subm.onblur; subm.onblur = undefined; subm.withFileSizeError = subm.onkeyup; subm.onkeyup = undefined; ",
                          "function checkFileSize(ev){if (ev.target && ev.target.files && ev.target.files[0] && ev.target.files[0].size > ", uw_Basis_htmlifyInt(ctx, maxFileSize) , "){subm.withFileSizeError(ev);}}",
                          "for (var node = subm.previousSibling; node.tagName != \"INPUT\"; node = node.previousSibling);",
                          "node.onchange = function(ev){checkFileSize(ev);};",

                          autoSubmit
                          ? "subm.style.visibility = \"hidden\"; node.onchange = function(ev) { var ok = checkFileSize(ev); if ok { subm.parentNode.submit(); if (subm.begin) { subm.begin(); } }; }; "
                          : "",
                          "</script>",
                          NULL);
}

uw_Basis_string uw_AjaxUploadFfi_notifySuccess(uw_context ctx, uw_Basis_string submitId, uw_Basis_int handle) {
  return uw_Basis_mstrcat(ctx,
                          "<script type=\"text/javascript\">var subm = window.top.document.getElementById(\"",
                          submitId,
                          "\"); window.top.event = {keyCode : ",
                          uw_Basis_htmlifyInt(ctx, handle),
                          "}; subm.withHandle(); </script>",
                          NULL);
}

uw_Basis_string uw_AjaxUploadFfi_notifyMimeTypeError(uw_context ctx, uw_Basis_string submitId) {
  return uw_Basis_mstrcat(ctx,
                          "<script type=\"text/javascript\">var subm = window.top.document.getElementById(\"",
                          submitId,
                          "\"); window.top.event = {}; subm.withMimeTypeError(); </script>",
                          NULL);
}
uw_Basis_string uw_AjaxUploadFfi_notifyFileSizeError(uw_context ctx, uw_Basis_string submitId) {
  return uw_Basis_mstrcat(ctx,
                          "<script type=\"text/javascript\">var subm = window.top.document.getElementById(\"",
                          submitId,
                          "\"); window.top.event = {}; subm.withFileSizeError(); </script>",
                          NULL);
}

uw_Basis_string uw_AjaxUploadFfi_idToString(uw_context ctx, uw_Basis_string s) {
  return s;
}

uw_Basis_string uw_AjaxUploadFfi_stringToId(uw_context ctx, uw_Basis_string s) {
  char *s2 = s;

  if (s2[0] == 'u' && s2[1] == 'w')
    s2 += 2;

  if (*s2 == '-')
    ++s2;

  for (++s2; *s2; ++s2)
    if (!isdigit(*s2))
      uw_error(ctx, FATAL, "AjaxUploadFfi.stringToId: Invalid ID");

  return s;
}
