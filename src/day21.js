#!/usr/bin/env node

function hrTime() {
    let t = process.hrtime()
    return Math.floor(t[0] * 1000000 + t[1] / 1000)
}
let start = hrTime()

let arrowCoords = {
    '^': [1,0],
    'A': [2,0],
    '<': [0,1],
    'v': [1,1],
    '>': [2,1],
}

let numericCoords = {
    '7': [0,0],
    '8': [1,0],
    '9': [2,0],
    '4': [0,1],
    '5': [1,1],
    '6': [2,1],
    '1': [0,2],
    '2': [1,2],
    '3': [2,2],
    '0': [1,3],
    'A': [2,3],
}

function explodeArrowPair(a, b) {
    let [ax, ay] = arrowCoords[a]
    let [bx, by] = arrowCoords[b]

    let dx = bx - ax
    let dy = by - ay

    let vArrow = dy >= 0 ? 'v' : '^'
    let hArrow = dx >= 0 ? '>' : '<'

    let vPart = vArrow.repeat(Math.abs(dy))
    let hPart = hArrow.repeat(Math.abs(dx))

    if (dx == 0) return vPart + 'A'
    if (dy == 0) return hPart + 'A'

    if ((a == 'A' || a == '^') && (b == '<')) {
        return vPart + hPart + 'A'
    }
    if ((b == 'A' || b == '^') && (a == '<')) {
        return hPart + vPart + 'A'
    }

    return {
        len: Math.abs(dx) + Math.abs(dy) + 1,
        left: vPart + hPart + 'A',
        right: hPart + vPart + 'A',
    }
}

function explodeNumberPair(a, b) {
    let [ax, ay] = numericCoords[a]
    let [bx, by] = numericCoords[b]

    let dx = bx - ax
    let dy = by - ay

    let vArrow = dy >= 0 ? 'v' : '^'
    let hArrow = dx >= 0 ? '>' : '<'

    let vPart = vArrow.repeat(Math.abs(dy))
    let hPart = hArrow.repeat(Math.abs(dx))

    if (dx == 0) return vPart + 'A'
    if (dy == 0) return hPart + 'A'

    if ((a == '0' || a == 'A') && (b == '1' || b == '4' || b == '7')) {
        return vPart + hPart + 'A'
    }
    if ((b == '0' || b == 'A') && (a == '1' || a == '4' || a == '7')) {
        return hPart + vPart + 'A'
    }

    return {
        len: Math.abs(dx) + Math.abs(dy) + 1,
        left: vPart + hPart + 'A',
        right: hPart + vPart + 'A',
    }
}

function minLength(elem) {
    let out = 0
    let go = (elem) => {
        if (Array.isArray(elem)) {
            for (let x of elem) {
                go(x)
            }
        }
        else if (typeof elem === 'string') {
            out += elem.length
        }
        else if (elem.left && elem.right) {
            out += elem.len
        }
        else throw new Error()
    }
    go(elem)
    return out
}

function flatten(elem) {
    let out = []
    let go = (elem) => {
        if (Array.isArray(elem)) {
            for (let x of elem) {
                go(x)
            }
        }
        else if (typeof elem === 'string') {
            out.push(elem)
        }
        else {
            out.push({
                len: elem.len,
                left: flatten(elem.left),
                right: flatten(elem.right)
            })
        }
    }
    go(elem)
    return out
}

function explodeArrows(elem) {
    if (Array.isArray(elem)) {
        return elem.map(explodeArrows)
    }
    if (typeof elem === 'string') {
        let sections = []
        for (let i = 0; i < elem.length; ++i) {
            sections.push(explodeArrowPair(i == 0 ? 'A' : elem[i - 1], elem[i]))
        }
        return sections
    }
    let left = explodeArrows(elem.left)
    let right = explodeArrows(elem.right)
    let leftLen = minLength(left)
    let rightLen = minLength(right)

    if (leftLen < rightLen) return left
    else if (leftLen > rightLen) return right
    return { left, right, len: leftLen }
}

function pickBestBranches(elem) {
    if (typeof elem === 'string') return elem

    let { left, right } = elem
    let ogLeft = left
    let ogRight = right

    for (;;) {
        left = flatten(explodeArrows(left))
        right = flatten(explodeArrows(right))

        let leftLen = minLength(left)
        let rightLen = minLength(right)

        if (leftLen < rightLen) return ogLeft
        if (leftLen > rightLen) return ogRight
    }
}

let numberPairToArrows = {}

let numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A']
for (let a = 0; a < numbers.length; ++a) {
    for (let b = 0; b < numbers.length; ++b) {
        numberPairToArrows[`${numbers[a]}${numbers[b]}`] = pickBestBranches(explodeNumberPair(numbers[a], numbers[b]))
    }
}

let arrowExpansions = {}

for (let k in numberPairToArrows) {
    let val = numberPairToArrows[k]

    if (!arrowExpansions[val]) {
        arrowExpansions[val] = explodeArrows(val).map(pickBestBranches)
    }
}

for (;;) {
    let foundANewOne = false

    for (let k in arrowExpansions) {
        let vals = arrowExpansions[k]
        for (let val of vals) {
            if (!arrowExpansions[val]) {
                foundANewOne = true
                arrowExpansions[val] = explodeArrows(val).map(pickBestBranches)
            }
        }
    }

    if (!foundANewOne) break
}

function addToBag(bag, str, count) {
    if (str in bag) bag[str] += count
    else bag[str] = count
}

function stepBag(bag) {
    let newBag = {}

    for (let str in bag) {
        let count = bag[str]
        let expansions = arrowExpansions[str]
        for (let val of expansions) {
            addToBag(newBag, val, count)
        }
    }

    return newBag
}

function countBag(bag) {
    let ret = 0
    for (let str in bag) {
        let count = bag[str]
        ret += str.length * count
    }
    return ret
}

function solve(numbers, depth) {
    let bag = {}

    numbers = 'A' + numbers
    for (let i = 0; i < numbers.length - 1; ++i) {
        addToBag(bag, numberPairToArrows[numbers.substr(i, 2)], 1)
    }

    for (let i = 0; i < depth; ++i) {
        bag = stepBag(bag)
    }

    return countBag(bag)
}

let part1 =
    solve('459A', 2) * 459 +
    solve('671A', 2) * 671 +
    solve('846A', 2) * 846 +
    solve('285A', 2) * 285 +
    solve('083A', 2) * 83

let part2 =
    solve('459A', 25) * 459 +
    solve('671A', 25) * 671 +
    solve('846A', 25) * 846 +
    solve('285A', 25) * 285 +
    solve('083A', 25) * 83

let time = hrTime() - start;

console.log('')
console.log('-- DAY 21 (javascript) --')
console.log(`   Time: ${time} μs`)
console.log(` Part 1: ${part1}`)
console.log(` Part 2: ${part2}`)
console.log('')
// Part 1: 169390
// Part 2: 210686850124870