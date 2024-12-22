#!/usr/bin/env node
const fs = require('fs')
// const input = fs.readFileSync('inputs/day21-test.txt')

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

    // return vPart + hPart + 'A'

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

    // return hPart + vPart + 'A'

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

function step(elem) {
    return flatten(explodeArrows(elem))
}

function explodeNumbers(str) {
    let sections = []
    for (let i = 0; i < str.length; ++i) {
        sections.push(explodeNumberPair(i == 0 ? 'A' : str[i - 1], str[i]))
    }

    for (let i = 0; i < 5; ++i) {
        console.log("Step", i)
        sections = step(sections)
    }

    console.log(JSON.stringify(sections))
    console.log(minLength(sections))
}

explodeNumbers('179A')