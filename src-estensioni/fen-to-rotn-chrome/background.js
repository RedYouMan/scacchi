
chrome.runtime.onInstalled.addListener(() => {
  chrome.contextMenus.create({
    id: "convert-fen",
    title: "Converti FEN in ROTN - Rosario Turco",
    contexts: ["page"]
  });
});

chrome.contextMenus.onClicked.addListener((info, tab) => {
  if (info.menuItemId === "convert-fen") {
    chrome.tabs.sendMessage(tab.id, {action: "convert"});
  }
});

