param(
    [Parameter(Mandatory = $true)]
    [string]$Path
)

$ErrorActionPreference = 'Stop'

function SetSelectionAfterKeyword([string[]]$keywords, $word, $selection) {
    foreach ($kw in $keywords) {
        $find = $selection.Range.Find
        $find.ClearFormatting()
        $find.Text = $kw
        $find.Forward = $true
        $find.Wrap = 1 # wdFindContinue
        if ($find.Execute()) {
            $selection.Collapse(0) # wdCollapseEnd
            $selection.TypeParagraph()
            return $true
        }
    }
    $selection.EndKey(6) # wdStory
    $selection.TypeParagraph()
    return $false
}

function AddTable($doc, $selection, [string[]]$headers, $rows) {
    $rowsCount = $rows.Count
    $cols = $headers.Count
    $table = $doc.Tables.Add($selection.Range, $rowsCount + 1, $cols)
    for ($c = 0; $c -lt $cols; $c++) { $table.Cell(1, $c + 1).Range.Text = $headers[$c] }
    for ($r = 0; $r -lt $rowsCount; $r++) {
        for ($c = 0; $c -lt $cols; $c++) { $table.Cell($r + 2, $c + 1).Range.Text = $rows[$r][$c] }
    }
    $table.Rows.Item(1).Range.Bold = $true
    $table.AllowAutoFit = $true
    $selection.MoveDown(5) # wdLine
    $selection.TypeParagraph()
}

try {
    $word = New-Object -ComObject Word.Application
    $word.Visible = $false
    $doc = $word.Documents.Open($Path)
    $sel = $word.Selection

    $keywords = @('System Testing', 'Testing', 'Test Cases')
    [void](SetSelectionAfterKeyword $keywords $word $sel)

    $sel.TypeText('Test Cases')
    $sel.TypeParagraph()
    $tcRows = @(
        @('Add Patient','In Patients page, fill name/gender/age/phone/address and confirm','Success dialog; list adds a row; total count +1',''),
        @('Create Appointment','Choose patient and doctor, set date 09:00, fill symptoms, confirm','Success dialog with appointment no; list adds a row',''),
        @('Edit Medicine Stock','Select a medicine, set stock to 5, confirm','Row updated; top shows Low Stock warning (red/bold)','')
    )
    AddTable $doc $sel @('Case','Input/Action','Expected Output','Actual Output') $tcRows

    $sel.TypeText('Screenshots')
    $sel.TypeParagraph()
    $shotRows = @(
        @('Add Patient','',''),
        @('Create Appointment','',''),
        @('Edit Medicine Stock','','')
    )
    AddTable $doc $sel @('Case','Screenshot File','Description/Location') $shotRows

    $sel.TypeText('Exception Tests')
    $sel.TypeParagraph()
    $exRows = @(
        @('Appointment time conflict','Create another appointment for same doctor/date/time','Dialog: conflict message; no insertion',''),
        @('No patient data','Empty patients table then export','CSV with headers only; patient count = 0',''),
        @('No doctor or patient','Open New Appointment','Dialog: no available patient/doctor; stop flow','')
    )
    AddTable $doc $sel @('Scenario','Input/Action','Expected Output','Actual Output') $exRows

    $doc.Save()
    $doc.Close()
    $word.Quit()
    Write-Output "OK: tables inserted into $Path"
} catch {
    try { if ($doc) { $doc.Close() } } catch {}
    try { if ($word) { $word.Quit() } } catch {}
    Write-Error $_
    exit 1
}
