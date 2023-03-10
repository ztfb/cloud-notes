package com.cloudnotes.service.controller;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.TextBlock;
import com.cloudnotes.service.service.NoteService;
import org.aspectj.weaver.ast.Not;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping(value = "/note")
@CrossOrigin
public class NoteController {
    @Autowired
    NoteService noteService;
    @GetMapping("/find")
    public List<Note> find(Integer cid){return noteService.find(cid);}
    @PostMapping("/create")
    public Result create(@RequestBody Note note){return noteService.create(note);}
    @GetMapping("/delete")
    public Result delete(Integer id){return noteService.delete(id);}
    @PostMapping("/update")
    public Result update(@RequestBody Note note){return noteService.update(note);}
    @GetMapping("/findText")
    public Result findText(Integer nid){return noteService.findText(nid);}
    @PostMapping("/updateText")
    public Result updateText(@RequestBody TextBlock textBlock){return noteService.updateText(textBlock);}
    @PostMapping("updatePower")
    public Result updatePower(@RequestBody Note note){return noteService.updatePower(note);}
    @PostMapping("/shareNew")
    public Result shareNew(@RequestBody Note note){return noteService.shareNew(note);}
}
