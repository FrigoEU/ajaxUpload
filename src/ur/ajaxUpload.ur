type handle = int

sequence handles

table scratch : { Handle : handle,
                  Filename : option string,
                  MimeType : string,
                  Content : blob,
                  Created : time }
  PRIMARY KEY Handle

(* Clean up files that go unclaimed for 30 minutes. *)
task periodic 900 = fn () =>
    tm <- now;
    dml (DELETE FROM scratch
         WHERE Created < {[addSeconds tm (-(30 * 60))]})

datatype claim_result =
         NotFound
       | Found of { Filename : option string,
                    MimeType : string,
                    Content : blob }

fun claim h =
    ro <- oneOrNoRows1 (SELECT scratch.Filename, scratch.MimeType, scratch.Content
                        FROM scratch
                        WHERE scratch.Handle = {[h]});
    case ro of
        None => return NotFound
      | Some r =>
        dml (DELETE FROM scratch
             WHERE Handle = {[h]});
        return (Found r)

fun peek h =
    ro <- oneOrNoRows1 (SELECT scratch.Filename, scratch.MimeType, scratch.Content
                        FROM scratch
                        WHERE scratch.Handle = {[h]});
    return (case ro of
                None => NotFound
              | Some r => Found r)

fun render {SubmitLabel = sl, OnBegin = ob, OnSuccess = os, OnMimeTypeError = onMTE, OnFileSizeError = onFSE, Accept = ac, MaxFileSize = maxFileSize} =
    iframeId <- fresh;
    submitId <- fresh;
    submitId' <- return (AjaxUploadFfi.idToString submitId);
    let
        fun uploadAction r =
            if Option.isNone (checkMime (fileMimeType r.File)) then
                return <xml><body>{AjaxUploadFfi.notifyMimeTypeError (AjaxUploadFfi.stringToId submitId')}</body></xml>
            else
                h <- nextval handles;
                dml (INSERT INTO scratch (Handle, Filename, MimeType, Content, Created)
                     VALUES ({[h]}, {[fileName r.File]}, {[fileMimeType r.File]}, {[fileData r.File]}, CURRENT_TIMESTAMP));
                return <xml><body>
                  {AjaxUploadFfi.notifySuccess (AjaxUploadFfi.stringToId submitId') h}
                </body></xml>
    in
        return <xml>
          <form>
            <upload{#File} accept={ac}/>
            <submit value={Option.get "" sl} action={uploadAction} id={submitId} onfocus={ob} onkeydown={fn ev => os ev.KeyCode} onblur={onMTE} onkeyup={fn _ => onFSE}/>
          </form>
          {AjaxUploadFfi.tweakForm (Option.isNone sl) iframeId submitId maxFileSize}
        </xml>
    end
