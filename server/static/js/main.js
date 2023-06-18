const video = document.getElementById('camera-video');
const inputItemId = document.getElementById('input-item-id');
const itemList = document.getElementById('item-list');
const beepAudio = new Audio('/audio/beep-6-96243.mp3');
let lastCode = 0;
let count = 0;

function scanBarcode(data) {
  let current = data.codeResult.code;
  
  if (lastCode == current) {
    count++;
  } else {
    count = 0;
  }

  if (count >= 10) {
    inputItemId.value = current;
    lastCode = 0;

    beepAudio.play();

  } else {
    lastCode = current;
  }
};

async function initPurchase() {
  try {
    await axios.get('/purchase')
  } catch {
    await axios.post('/purchase')
  }

  updatePurchase();
}

function initScanner() {
  navigator
    .mediaDevices
    .getUserMedia({
      video: true,
      audio: false
    })
    .then((stream) => {
      video.srcObject = stream;
      video.play();
    })
    .catch((err) => {
      console.error(err);
    });

    Quagga.init({
      inputStream: {
        name: "Live",
        type: "LiveStream",
        target: video
      },
      decoder: {
        readers: [
          "code_128_reader",
          "ean_reader",
          "ean_8_reader",
          "code_39_reader",
          "code_39_vin_reader",
          "codabar_reader",
          "upc_reader",
          "upc_e_reader",
          "i2of5_reader",
          "2of5_reader",
          "code_93_reader",
        ]
      }
    }, (err) => {
      if (err) {
        console.error(err);
        return
      }

    Quagga.start();
    Quagga.onDetected(scanBarcode);
  });
}

async function updatePurchase(frame) {
  itemList.innerHTML = '';

  let result = await axios.get('/purchase');
  let purchases = result.data.purchases;

  for (const purchase of purchases) {
    let html = `
    <a
      href="#"
      class="list-group-item list-group-item-action d-flex justify-content-between align-items-center"
      onClick="deleteItem(${purchase.ItemID});"
    >
      ${purchase.ItemName}
      <span class="badge bg-primary rounded-pill">${purchase.Count}</span>
    </a>`;

    itemList.innerHTML += html;
  }
}

async function deletePurchase() {
  if (confirm('Do you continue?')) {
    await axios.delete('/purchase');
    location.reload();
  }
}

async function syncPurchase() {
  let syncId = prompt("Sync ID", "");

  await axios.post(`/sync/${syncId}`);

  alert('Sync complete. Continue to checkout!');
}

async function addItem() {
  const value = inputItemId.value;

  if (value == '') {
    alert('No item');
    return;
  }

  try {
    await axios.post("/purchase/item", {
      ItemID: value,
      count: 1
    });

    updatePurchase();

  } catch (error) {
    if (error.response.status === 500) {
      alert('This product does not exist!');
    } else {
      alert('Error!');
    }
  }
}

function cancelItem() {
  inputItemId.value = '';
}

async function deleteItem(id) {
  await axios.delete("/purchase/item", {
    data: {
      id: id.toString(),
      count: 1
    }
  });

  updatePurchase();
}

(() => {
  initPurchase();
  initScanner();
})();