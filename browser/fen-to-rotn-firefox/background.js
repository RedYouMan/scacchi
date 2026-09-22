
browser.commands.onCommand.addListener(async (command) => {
  if (command!== "convert-fen") return;
  const [tab] = await browser.tabs.query({active: true, currentWindow: true});
  browser.tabs.sendMessage(tab.id, {action: "convert"});
});

