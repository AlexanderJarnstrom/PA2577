const Statistic = require("./Statistic");

class StatisicsStorage {
  static #instance = null;

  #averageHundred = null;
  #averageThousand = null;
  #averageAll = null;

  #averageEveryHundredData = [];
  #averageEveryHundredPoint = [];
  #averageEveryHundredCounter = 0;

  static getInstance() {
    StatisicsStorage.#instance = StatisicsStorage.#instance || new StatisicsStorage();
    return StatisicsStorage.#instance;
  }

  constructor() {
    this.#averageAll = new Statistic(-1);
    this.#averageHundred = new Statistic(100);
    this.#averageThousand = new Statistic(1000);
  }

  get averageHundred() { return this.#averageHundred; }
  get averageThousand() { return this.#averageThousand; }
  get average() { return this.#averageAll; }

  addTime(file) {
    const times = file.timers;
    this.#averageAll.addTime(times);
    this.#averageThousand.addTime(times);
    this.#averageHundred.addTime(times);

    return file;
  }

  noteAverage() {
    this.#averageEveryHundredCounter += 100;
    this.#averageEveryHundredData.push(this.#averageHundred.averageMatch / 1000);
    this.#averageEveryHundredPoint.push(this.#averageEveryHundredCounter);
  }

  getGraphData() {
    let output = '';
    let data = '[';
    for (let i = 0; i < this.#averageEveryHundredData.length; i++) {
      data += this.#averageEveryHundredData[i] + ',';
    }
    data = data.replace(/.$/, ']');

    let points = '[';
    for (let i = 0; i < this.#averageEveryHundredPoint.length; i++) {
      points += this.#averageEveryHundredPoint[i] + ',';
    }
    points = points.replace(/.$/, ']');

    output += 'labels: ' + points + ', datasets: [{ label: \'Average Match Time (µs)\', data: ' + data + ' }]';

    return output;
  }

  getOutput() {
    let output = '';

    output += '<p>Average for last 100 files (' + this.#averageHundred.size + '):</p>\n<ul>\n';
    output += '<li>Total: ' + (this.#averageHundred.averageTotal / 1000).toFixed(2) + " µs</li>\n";
    output += '<li>Match: ' + (this.#averageHundred.averageMatch / 1000).toFixed(2) + " µs</li>\n";
    output += '</ul>\n'

    output += '<p>Average for last 1000 files (' + this.#averageThousand.size + '):</p>\n<ul>\n';
    output += '<li>Total: ' + (this.#averageThousand.averageTotal / 1000).toFixed(2) + " µs</li>\n";
    output += '<li>Match: ' + (this.#averageThousand.averageMatch / 1000).toFixed(2) + " µs</li>\n";
    output += '</ul>\n'

    output += '<p>Average for all files (' + this.#averageAll.size + '):</p>\n<ul>\n';
    output += '<li>Total: ' + (this.#averageAll.averageTotal / 1000).toFixed(2) + " µs</li>\n";
    output += '<li>Match: ' + (this.#averageAll.averageMatch / 1000).toFixed(2) + " µs</li>\n";
    output += '</ul>\n'

    output += '<p>Current memory usage: ' + Math.round(process.memoryUsage().heapUsed / 1024 / 1024 * 100) / 100 + 'MB </p>'

    return output;
  }
}

module.exports = StatisicsStorage;
