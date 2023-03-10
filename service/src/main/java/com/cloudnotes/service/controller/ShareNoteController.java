package com.cloudnotes.service.controller;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.ShareRelation;
import com.cloudnotes.service.service.ShareNoteService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping(value = "/shareNote")
@CrossOrigin
public class ShareNoteController {
    @Autowired
    ShareNoteService shareNoteService;
    @GetMapping("/delete")
    public Result delete(Integer id){return shareNoteService.delete(id);}
    @GetMapping("find")
    public List<ShareRelation> find(Integer sid){return shareNoteService.find(sid);}
}
